#include "StaticModels.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include "TriangleRenderable.h"
#include <iostream>
#include "SceneManager.h"
#include "Material.h"
#include "Octree.h"

StaticModels::StaticModels(SceneManager* pRenderingSytem)
    :m_pSceneManager(pRenderingSytem)
    ,m_octreeMesh(nullptr)
{
}


StaticModels::~StaticModels()
{
}

AABB StaticModels::getBoundingBox()
{
    AABB ret;
    for (auto pMesh : m_staticMeshes)
    {
        AABB tmpBox = pMesh->getBoundingBox();
        ret.merge(tmpBox);
    }
    return ret;
}

void StaticModels::buildOctree()
{
    if (m_octreeRoot == nullptr)
    {
        m_octreeRoot = new OctreeNode(getBoundingBox());
        for (auto pMesh : m_staticMeshes)
        {
            TriangleMesh* pTriangleMesh = dynamic_cast<TriangleMesh*>(pMesh);
            if (pTriangleMesh)
            {
                auto tessellationData = pTriangleMesh->getTessellationData();

                for (size_t i = 0, n = tessellationData.m_indices.size(); i < n; i += 3)
                {
                    int idx0 = tessellationData.m_indices[i];
                    int idx1 = tessellationData.m_indices[i+1];
                    int idx2 = tessellationData.m_indices[i+2];

                    m_octreeRoot->addTriangle(Triangle{ tessellationData.m_vertices[idx0],
                                                        tessellationData.m_vertices[idx1],
                                                        tessellationData.m_vertices[idx2] }
                        );
                }
               
            }
        }
        m_octreeRoot->divide();
    }
}

void StaticModels::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path, 0);

    CstPointSequence vertices;
    CstInt32Sequence indices;
    CstPointSequence normals;

    for (unsigned i = 0; i < pScene->mNumMeshes; ++i)
    {

        const aiMesh* pMesh = pScene->mMeshes[i];

        vertices.clear();
        vertices.resize(pMesh->mNumVertices);
        memcpy(vertices.w_address(), pMesh->mVertices, sizeof(vect3)*pMesh->mNumVertices);


        indices.clear();
        int indicesNumer = 0;
        for (unsigned j = 0; j < pMesh->mNumFaces; ++j)
        {
            indicesNumer += pMesh->mFaces[j].mNumIndices;
        }

        indices.resize(indicesNumer);
        unsigned* pDest = (unsigned*)indices.w_address();
        for (unsigned j = 0; j < pMesh->mNumFaces; ++j)
        {
            unsigned indicesCount = pMesh->mFaces[j].mNumIndices;
            memcpy(pDest, pMesh->mFaces[j].mIndices, sizeof(unsigned)*indicesCount);
            pDest += indicesCount;
        }

        normals.clear();
        if (pMesh->HasNormals())
        {
            normals.resize(pMesh->mNumVertices);
            memcpy(normals.w_address(), pMesh->mNormals, sizeof(vect3)*pMesh->mNumVertices);
        }


        MaterialManager* pMaterialMgr = m_pSceneManager->getMaterialManager();
        IMesh* pTriangleMesh = m_pSceneManager->createTriangleMesh(vertices, indices, normals);
        //pTriangleMesh->setRenderMethod(pMaterialMgr->getShadedTriangleRenderMethod());
        pTriangleMesh->setRenderMethod(pMaterialMgr->getWireFrameTriangleRenderMethod());
        m_staticMeshes.insert(pTriangleMesh);
    }
}

void StaticModels::drawOctree()
{
    if (m_octreeRoot != nullptr && m_octreeMesh == nullptr)
    {
        TessellationData data = m_octreeRoot->getTessellationData(-1);
        MaterialManager* pMaterialMgr = m_pSceneManager->getMaterialManager();
        m_octreeMesh = m_pSceneManager->createLineMesh(data.m_vertices, data.m_indices);
        m_octreeMesh->setRenderMethod(pMaterialMgr->getSimpleLineRenderMethod());
    }
}

bool StaticModels::segment_intersection(const vect3& p1, const vect3& p2, vect3 hitPos)
{
    if (!m_octreeRoot )
        return false;

    float t;
    if (m_octreeRoot->segment_intersection(p1, p2, t))
    {
        hitPos = p1 + (p1 - p2)*t;
        return true;
    }
    return false;
}