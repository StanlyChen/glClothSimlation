#include "ClothPhysical.h"
#include "SceneManager.h"
#include "Material.h"
#include <iostream>
#include "StaticModels.h"


//TODO SHEAR FORCE
//TODO BLEND FORCE
//TODO ADVANCED INTEGRATE
//TODO COLLION

const int NET_WIDTH = 21;
const int NET_HEIGHT = 21;

const REAL PARTICLE_MASS = REAL(0.01);
const REAL MESH_CETNER_X = 0;
const REAL MESH_CENTER_Z = 0;
const REAL init_height = 10.f;




const vect3 GRAVITY = vect3(0, REAL(-9.8), 0);
const vect3 WIND = vect3(0,0,1);
const REAL AREA_K = REAL(0.1);

const REAL DELTA_TIME = 0.001f;
const REAL SPRING_CONSTANT = REAL(50);
const REAL DAMPER_FACTOR = REAL(2);


void SpringDamper::ComputeForce()
{

    auto distance = glm::distance(mP2->position, mP1->position);
    auto vn_p2ToP1 = glm::normalize(mP1->position - mP2->position);

    auto v_speedP1 = glm::dot(mP1->vecolity, vn_p2ToP1);
    auto v_speedP2 = glm::dot(mP2->vecolity, vn_p2ToP1);

    auto f_spring = -mSpringConstant * ( distance - mRestLength);
    auto f_damping = -mDampingFactor * (v_speedP1 - v_speedP2);

    auto f_springDampter = vn_p2ToP1 * (f_spring +  f_damping);

    mP1->force += f_springDampter;
    mP2->force += -f_springDampter;
}


ClothPhysical::ClothPhysical(SceneManager* pRenderSystem, StaticModels* pStaticModelManager)
{
    m_pSceneManager = pRenderSystem;
    m_pStaticModelsManager = pStaticModelManager;
}


ClothPhysical::~ClothPhysical()
{
}

void ClothPhysical::init()
{
    //init partiles
    m_particles.resize(NET_WIDTH* NET_HEIGHT);

    //const REAL MESH_SIZE = 0.3f;
    //const REAL CLOTH_WIDTH = 10;
    const REAL CLOTH_WIDTH = m_pStaticModelsManager->getBoundingBox().diagonalLength();
    const REAL CLOTH_HEIGHT = CLOTH_WIDTH;


    //REAL MESH_SIZE_WIDTH = MESH_SIZE;
    //REAL MESH_SIZE_HEIGHT = MESH_SIZE;

    REAL MESH_SIZE_WIDTH = CLOTH_WIDTH/(NET_WIDTH-1);
    REAL MESH_SIZE_HEIGHT = CLOTH_HEIGHT/(NET_HEIGHT-1);

    REAL start_x = MESH_CETNER_X - NET_WIDTH * MESH_SIZE_WIDTH / 2;
    REAL start_z = MESH_CENTER_Z - NET_HEIGHT * MESH_SIZE_HEIGHT / 2;


    for (int r = 0; r < NET_HEIGHT; ++r)
    {
        for (int c = 0; c < NET_WIDTH; ++c)
        {
            ClothParticle& particle = m_particles[r*NET_WIDTH + c];
            vect3& p = particle.position;
            p.x = start_x + c * MESH_SIZE_WIDTH;
            p.y = init_height;
            p.z = start_z + r * MESH_SIZE_HEIGHT;

            particle.vecolity = vect3(0);
            particle.force = vect3(0);
            particle.acc = vect3(0);
            particle.mass = PARTICLE_MASS;
        }
    }
    m_assitant_PArray.assign(m_particles.begin(), m_particles.end());

    m_triangleIndices.reserve(max(0, (NET_WIDTH - 1)*(NET_HEIGHT - 1) * 6));
    for (int r = 0; r < NET_HEIGHT - 1; ++r)
    {
        for (int c = 0; c < NET_WIDTH - 1; ++c)
        {
            auto idx1 = r * NET_WIDTH + c;
            auto idx2 = (r + 1)*NET_WIDTH + c;
            auto idx3 = (r + 1)*NET_WIDTH + (c + 1);

            auto idx4 = idx1;
            auto idx5 = idx3;
            auto idx6 = r * NET_WIDTH + (c + 1);

            m_triangleIndices.push_back(idx1);
            m_triangleIndices.push_back(idx2);
            m_triangleIndices.push_back(idx3);

            m_triangleIndices.push_back(idx4);
            m_triangleIndices.push_back(idx5);
            m_triangleIndices.push_back(idx6);
        }
    }

    //init meshes
    CstPointSequence vertices, normals;
    vertices.reserve(m_particles.size());
    normals.reserve(m_particles.size());
    m_assitant_V3Array.reserve(m_particles.size());

    for (const auto& p : m_particles)
    {
        m_assitant_V3Array.push_back(p.position);
        vertices.push_back(p.position);
        normals.push_back(vect3(0, 1, 0));
    }

    MaterialManager* pMaterialMgr = m_pSceneManager->getMaterialManager();
    //m_pMesh = m_pRenderSystem->createPointMesh(vertices);
    //m_pMesh->setRenderMethod(pMaterialMgr->getSimplePointRenderMethod());

    m_pMesh = m_pSceneManager->createTriangleMesh(vertices, m_triangleIndices, normals);
    m_pMesh->setRenderMethod(pMaterialMgr->getClothTriangleRenderMethod());
    //m_pMesh->setRenderMethod(pMaterialMgr->getWireFrameTriangleRenderMethod(vect3{1,0,0}));

    //init SpringDamper
    m_springDampers.reserve(2 * (NET_HEIGHT - 1)*(NET_WIDTH - 1));
    for (int r = 0; r < NET_HEIGHT; ++r)
    {
        for (int c = 0; c < NET_WIDTH ; ++c)
        {
            ClothParticle& particle_cur = m_particles[r*NET_WIDTH + c];

            if (c + 1 < NET_WIDTH)
            {
                ClothParticle& particle_left = m_particles[r*NET_WIDTH + c + 1];

                SpringDamper sd1;
                sd1.mP1 = &particle_cur;
                sd1.mP2 = &particle_left;
                sd1.mSpringConstant = SPRING_CONSTANT;
                sd1.mDampingFactor = DAMPER_FACTOR;
                sd1.mRestLength = glm::distance(particle_cur.position, particle_left.position);

                m_springDampers.push_back(sd1);
            }

            if ((r + 1) < NET_HEIGHT)
            {
                ClothParticle& particle_down = m_particles[(r + 1)*NET_WIDTH + c];

                SpringDamper sd2;
                sd2.mP1 = &particle_cur;
                sd2.mP2 = &particle_down;
                sd2.mSpringConstant = SPRING_CONSTANT;
                sd2.mDampingFactor = DAMPER_FACTOR;
                sd2.mRestLength = glm::distance(particle_cur.position, particle_down.position);

                m_springDampers.push_back(sd2);
            }

            if ((r + 1) < NET_HEIGHT && (c - 1) > 0)
            {
                ClothParticle& particle_left_across = m_particles[(r + 1)*NET_WIDTH + c-1];

                SpringDamper sd3;
                sd3.mP1 = &particle_cur;
                sd3.mP2 = &particle_left_across;
                sd3.mSpringConstant = SPRING_CONSTANT;
                sd3.mDampingFactor = DAMPER_FACTOR;
                sd3.mRestLength = glm::distance(particle_cur.position, particle_left_across.position);

                m_springDampers.push_back(sd3);
            }

            if ((r + 1) < NET_HEIGHT && (c + 1) < NET_WIDTH)
            {
                ClothParticle& particle_right_across = m_particles[(r + 1)*NET_WIDTH + c + 1];

                SpringDamper sd4;
                sd4.mP1 = &particle_cur;
                sd4.mP2 = &particle_right_across;
                sd4.mSpringConstant = SPRING_CONSTANT;
                sd4.mDampingFactor = DAMPER_FACTOR;
                sd4.mRestLength = glm::distance(particle_cur.position, particle_right_across.position);

                m_springDampers.push_back(sd4);
            }
 
        }
    }



}

void ClothPhysical::computer_force()
{
    //apply gravity
    for (auto &particle : m_particles)
    {
        particle.force = GRAVITY * particle.mass;
    }

    //spring damper
    for (auto &springDamper : m_springDampers)
    {
        springDamper.ComputeForce();
    }

    //aerodynamic force
    for (size_t i = 0, n = m_triangleIndices.size(); i < n; i += 3)
    {
        auto& p1 = m_particles[m_triangleIndices[i]];
        auto& p2 = m_particles[m_triangleIndices[i+1]];
        auto& p3 = m_particles[m_triangleIndices[i+2]];

        auto airForce = HelpUtil::calcuateAeraDynamic(p1.position, p1.vecolity, p2.position,p2.vecolity, p3.position,p3.vecolity, WIND);
        p1.force += airForce / REAL(3);
        p2.force += airForce / REAL(3);
        p3.force += airForce / REAL(3);
    }
}




void ClothPhysical::computer_constraint()
{
    for (int r = 0; r < 1; ++r)
    {
        for (int c = 0; c < NET_WIDTH; ++c)
        {
            ClothParticle& particle_cur = m_particles[r*NET_WIDTH + c];
            particle_cur.force = vect3(0);
        }
    }
}

vect3 HelpUtil::calcuateAeraDynamic(const vect3& p1_pos, const vect3& p1_vel, const vect3& p2_pos, const vect3& p2_vel, const vect3& p3_pos, const vect3& p3_vel, vect3 airSpeed)
{
    auto vf = (p1_vel + p2_vel + p3_vel) / REAL(3);
    auto v = vf - airSpeed;
    auto vm = glm::length(v);

    auto tmp = glm::cross(p2_pos - p1_pos, p3_pos - p1_pos);
    auto n = glm::normalize(tmp);
    auto f_area = glm::length(tmp) / 2;

    if (glm::dot(n, v) > 0)
        n = -n;

    if (vm != 0)
    {
        auto fc_area = std::abs(f_area * glm::dot(n, v) / vm);
        REAL k = AREA_K;// 0.5*p*c
        auto f_areaDynamic = k * fc_area*vm*vm;

        return f_areaDynamic * n;
    }
}

void HelpUtil::calcuateAeraDynamic(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3, vect3 airSpeed)
{
    auto vf = (p1->vecolity + p2->vecolity + p3->vecolity) / REAL(3);
    auto v = vf - airSpeed;
    auto vm = glm::length(v);

    auto tmp =  glm::cross(p2->position - p1->position, p3->position - p1->position);
    auto n = glm::normalize(tmp);
    auto f_area = glm::length(tmp) / 2;

    if (glm::dot(n, v) > 0)
        n = -n;

    if (vm != 0)
    {
        auto fc_area = std::abs(f_area * glm::dot(n, v) / vm);
        REAL k = AREA_K;// 0.5*p*c
        auto f_areaDynamic = k * fc_area*vm*vm;

        auto fv_areaDynamic = f_areaDynamic / REAL(3) * n;
        p1->force += fv_areaDynamic;
        p2->force += fv_areaDynamic;
        p3->force += fv_areaDynamic;

        if (fv_areaDynamic.y > 10)
            std::cout << "Error" << std::endl;
    }

}

void ClothPhysical::integrate_motion1(REAL time)
{
    m_assitant_PArray.swap(m_particles);
    for (auto &partial : m_particles)
    {
        //update acceleration
        auto acc = partial.force / partial.mass;
        partial.acc = acc;

        //update velocity
        partial.vecolity += acc* time;

        //update position
        auto oldPos = partial.position;
        auto newPos = oldPos + partial.vecolity *time;

        HelpUtil::calcuateCollision(m_pStaticModelsManager, oldPos, newPos, partial.position);
    }
}

void ClothPhysical::integrate_motion2(REAL time)
{
    for (auto &partial : m_particles)
    {
        //update acceleration
        auto acc0 = partial.acc;

        auto acc = partial.force / partial.mass;
        partial.acc = acc;

        //update velocity
        auto vec0 = partial.vecolity;
        partial.vecolity += (acc + acc0)* REAL(0.5) * time;

        //update position
        auto oldPos = partial.position;
        auto newPos = oldPos + (partial.vecolity + vec0)* REAL(0.5)*time;

        HelpUtil::calcuateCollision(m_pStaticModelsManager, oldPos, newPos, partial.position);
    }
}

bool HelpUtil::calcuateCollision(StaticModels* pStaticModels, const vect3& p1, const vect3& p2, vect3& hitPos)
{
    if (pStaticModels->segment_intersection(p1, p2, hitPos))
    {
        hitPos = p1;
        return true;
    }
    else
    {
        hitPos = p2;
        return false;
    }

    //{
    //    std::cout << "hit" << std::endl;
    //}

    //auto highY = REAL(0);
    //if ( p1.y != p2.y && (p1.y - highY)*(p2.y - highY) <= 0 )
    //{
    //    hitPos = p1 + (p2 - p1)*(highY - p1.y)/(p2.y - p1.y);
    //    return true;
    //}
    //else
    //{
    //    hitPos = p2;
    //    return false;
    //}
}

 vect3 HelpUtil::calcauteSpringDamperForce(const vect3& p1_pos, const vect3& p1_vel, const vect3& p2_pos, const vect3& p2_vel, REAL restLength, REAL springConstant, REAL damperFactor)
{
     auto distance = glm::distance(p1_pos, p2_pos);
     auto vn_p2ToP1 = glm::normalize(p1_pos - p2_pos);

     auto v_speedP1 = glm::dot(p1_vel, vn_p2ToP1);
     auto v_speedP2 = glm::dot(p2_vel, vn_p2ToP1);

     auto f_spring = -springConstant * (distance - restLength);
     auto f_damping = -damperFactor * (v_speedP1 - v_speedP2);

     auto f_springDampter = vn_p2ToP1 * (f_spring + f_damping);

     return f_springDampter;
}

void ClothPhysical::update(REAL deltaTime)
{
    deltaTime = 0.01f;

    REAL tmpDelatTime = 0;
    for (; tmpDelatTime < deltaTime; tmpDelatTime += DELTA_TIME)
    {
        //update particle
        computer_force();
        //computer_constraint();
        integrate_motion1(DELTA_TIME);
    }

    //update mesh

    CstPointSequence vertices, normals;
    vertices.reserve(m_particles.size());
    normals.resize(m_particles.size(),vect3(0));

    for (const auto& p : m_particles)
    {
        vertices.push_back(p.position);
    }

    //update normals
    for (size_t i = 0, n = m_triangleIndices.size(); i < n; i += 3)
    {
        int idx1 = m_triangleIndices[i];
        int idx2 = m_triangleIndices[i+1];
        int idx3 = m_triangleIndices[i+2];

        vect3 p1 = vertices[idx1];
        vect3 p2 = vertices[idx2];
        vect3 p3 = vertices[idx3];

        auto tmp = glm::cross(p2 - p1, p3 - p1);
        auto normal = glm::normalize(tmp);

        normals[idx1] += normal;
        normals[idx2] += normal;
        normals[idx3] += normal;
    }

    for (vect3& normal : normals)
        normal = glm::normalize(normal);

    if (m_pMesh)
    {
        m_pMesh->updateVertices(vertices,normals);
    }

}