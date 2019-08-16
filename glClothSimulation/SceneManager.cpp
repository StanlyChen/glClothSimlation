#include "SceneManager.h"
#include "PointRenderable.h"
#include "TriangleRenderable.h"
#include "LineRenderable.h"

SceneManager::SceneManager(RenderSystem* pRenderingSystem)
{
    m_pRenderingSystem = pRenderingSystem;
}

IMesh* SceneManager::createPointMesh(const CstPointSequence& verteices)
{
   
    PointMesh* pPointMesh = new PointMesh(this, verteices);

    m_pRenderingSystem->addRenderable(pPointMesh->getRenderObject());
    m_meshes.insert(pPointMesh);

    return (IMesh*)pPointMesh;
}

IMesh* SceneManager::createLineMesh(const CstPointSequence& verteices, const CstInt32Sequence& indices)
{
    LineMesh* pLineMesh = new LineMesh(this, verteices, indices);

    m_pRenderingSystem->addRenderable(pLineMesh->getRenderObject());
    m_meshes.insert(pLineMesh);

    return (IMesh*)pLineMesh;
}

IMesh* SceneManager::createTriangleMesh(const CstPointSequence& verteices, const CstInt32Sequence& indices, const CstPointSequence& normals)
{
    TriangleMesh* pTriangleMesh = new TriangleMesh(this, verteices, indices, normals);

    m_pRenderingSystem->addRenderable(pTriangleMesh->getRenderObject());
    m_meshes.insert(pTriangleMesh);

    return (IMesh*)pTriangleMesh;
}

void SceneManager::destoryMesh( IMesh* pMesh)
{
    if (m_meshes.erase(pMesh))
    {
        m_pRenderingSystem->removeRenderable(pMesh->getRenderObject());
        delete pMesh;
    }
}

MaterialManager* SceneManager::getMaterialManager() const
{
    return m_pRenderingSystem->getMaterialManager();
}

PointMesh::PointMesh(SceneManager* pSceneManger, const CstPointSequence& points)
    :IMesh(pSceneManger)
{
    m_box = getBoundingBox(points);
    m_pRenderObj = new PointRenderable();
    m_pRenderObj->initRenderingData( points);

    m_tessellationData.m_primType = POINT_PRIMITVIE;
    m_tessellationData.m_vertices = points;
}

PointMesh::~PointMesh()
{
    if (m_pRenderObj)
    {
        m_pRenderObj->destoryRenderingData();
        delete m_pRenderObj;
        m_pRenderObj = nullptr;
    }
}


void PointMesh::updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals)
{
    if (m_pRenderObj)
        m_pRenderObj->updateVertices( vertices);
};


void PointMesh::setRenderMethod(IRenderMethod* pMethod)
{
    if (m_pRenderObj)
        m_pRenderObj->setRenderMethod(pMethod);
}


TriangleMesh::TriangleMesh(SceneManager* pSceneManager, const CstPointSequence& vertices, const CstInt32Sequence& indices, const CstPointSequence& normals)
    :IMesh(pSceneManager)
{
    m_box = getBoundingBox(vertices);
    m_pRenderObj = new TriangleRenderable();
    m_pRenderObj->initRenderingData(vertices, indices, normals);

    m_tessellationData.m_primType = TRIANGLE_PRIMITIVE;
    m_tessellationData.m_vertices = vertices;
    m_tessellationData.m_indices = indices;
    m_tessellationData.m_normals = normals;
}

TriangleMesh::~TriangleMesh()
{
    if (m_pRenderObj)
    {
        m_pRenderObj->destoryRenderingData();
        delete m_pRenderObj;
        m_pRenderObj = nullptr;
    }
}

void TriangleMesh::updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals)
{
    if (m_pRenderObj)
        m_pRenderObj->updateVertices(vertices, normals);
};

void TriangleMesh::setRenderMethod(IRenderMethod* pMethod)
{
    if (m_pRenderObj)
        m_pRenderObj->setRenderMethod(pMethod);
}

LineMesh::LineMesh(SceneManager* pSceneManager, const CstPointSequence& vertices, const CstInt32Sequence& indices)
    :IMesh(pSceneManager)
{
    m_box = getBoundingBox(vertices);
    m_pRenderObj = new LineRenderable();
    m_pRenderObj->initRenderingData(vertices, indices);

    m_tessellationData.m_primType = LINE_PRIMITIVE;
    m_tessellationData.m_vertices = vertices;
    m_tessellationData.m_indices = indices;

}

LineMesh::~LineMesh()
{
    if (m_pRenderObj)
    {
        m_pRenderObj->destoryRenderingData();
        delete m_pRenderObj;
        m_pRenderObj = nullptr;
    }
}

void LineMesh::updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals)
{
    if (m_pRenderObj)
        m_pRenderObj->updateVertices(vertices);
};

void LineMesh::setRenderMethod(IRenderMethod* pMethod)
{
    if (m_pRenderObj)
        m_pRenderObj->setRenderMethod(pMethod);
}