#pragma once
#include <vector>
#include "PhysicalType.h"

class SceneManager;
class StaticModels;
class IMesh;



struct ClothParticle
{
    float mass;
    vect3 acc;
    vect3 position;
    vect3 vecolity;
    vect3 force;
};

struct SpringDamper
{
    REAL mSpringConstant;
    REAL mDampingFactor;
    REAL mRestLength;
    ClothParticle* mP1;
    ClothParticle* mP2;

    void ComputeForce();
};

class HelpUtil
{
public:
    static vect3 calcauteSpringDamperForce(const vect3& p1_pos, const vect3& p1_vel, const vect3& p2_pos, const vect3& p2_vel, REAL restLength, REAL springFactor, REAL damperFactor);
    static void calcuateAeraDynamic(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3, vect3 airSpeed);
    static vect3 calcuateAeraDynamic(const vect3& p1_pos, const vect3& p1_vel, const vect3& p2_pos,  const vect3& p2_vel, const vect3& p3_pos, const vect3& p3_vel, vect3 airSpeed);
    static bool calcuateCollision(StaticModels* pModelManager, const vect3& p1, const vect3& p2, vect3& hitPos);
};


class ClothPhysical
{
public:
    ClothPhysical(SceneManager* pSceneManager, StaticModels* pStaticModels);
    ~ClothPhysical();


public:
    void init();

    void update(REAL deltaTime);

    
private:
    void computer_force();
    void computer_constraint();

    void integrate_motion1(REAL time);
    void integrate_motion2(REAL time);



private:
     SceneManager* m_pSceneManager;
     StaticModels* m_pStaticModelsManager;
     std::vector<ClothParticle> m_particles;

     CstInt32Sequence      m_triangleIndices;
     std::vector< SpringDamper> m_springDampers;
     IMesh*                m_pMesh;


     std::vector<ClothParticle> m_assitant_PArray;
     std::vector<vect3>         m_assitant_V3Array;

};

