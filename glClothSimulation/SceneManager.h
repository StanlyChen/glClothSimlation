#pragma once
#include <vector>
#include <glm.hpp>
#include <set>
#include "RenderSystem.h"
#include "AABB.h"

class SceneManager;
class PointRenderable;
class TriangleRenderable;
class LineRenderable;
class IRenderable;




class IMesh
{
public:
    IMesh(SceneManager* pSceneManger)
        : m_pSceneManager(pSceneManger)
    {}

    virtual void updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals) = 0;
    virtual IRenderable* getRenderObject() = 0;
    virtual AABB getBoundingBox() { return m_box; };
    virtual void setRenderMethod(IRenderMethod* pMethod) = 0;

    static AABB getBoundingBox(const CstPointSequence& vertices) 
    {
        AABB ret;
        for (const vect3& vertex : vertices)
            ret.merge(vertex);
        return ret;
    }

    const TessellationData& getTessellationData()
    {
        return m_tessellationData;
    }

protected:
    SceneManager*    m_pSceneManager;
    AABB             m_box;
    TessellationData m_tessellationData;
};

class PointMesh : public IMesh
{
public:
    PointMesh(SceneManager* pSceneManger, const CstPointSequence& points);
    ~PointMesh();

    void updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals) override;

    void setRenderMethod(IRenderMethod* pMethod) override;

    IRenderable* getRenderObject() override
    {
        return (IRenderable*)m_pRenderObj;
    }
private:
    PointRenderable* m_pRenderObj;

};


class TriangleMesh : public IMesh
{
public:
    TriangleMesh(SceneManager* pSceneManager, const CstPointSequence& points, const CstInt32Sequence& indices, const CstPointSequence& normals);
    ~TriangleMesh();

    void updateVertices(const CstPointSequence& vertices,const CstPointSequence& normals) override;
    void setRenderMethod(IRenderMethod* pMethod) override;

    IRenderable* getRenderObject() override
    {
        return (IRenderable*)m_pRenderObj;
    }

private:
    TriangleRenderable* m_pRenderObj;
};

class LineMesh : public IMesh
{
public:
    LineMesh(SceneManager* pSceneManager, const CstPointSequence& points, const CstInt32Sequence& indices);
    ~LineMesh();

    void updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals) override;
    void setRenderMethod(IRenderMethod* pMethod) override;

    IRenderable* getRenderObject() override
    {
        return (IRenderable*)m_pRenderObj;
    }
private:
    LineRenderable* m_pRenderObj;
};

class SceneManager
{
public:
    SceneManager(RenderSystem* pRenderingSystem);


    IMesh* createPointMesh(const CstPointSequence& verteices);
    IMesh* createLineMesh(const CstPointSequence& verteices, const CstInt32Sequence& indices);
    IMesh* createTriangleMesh(const CstPointSequence& verteices, const CstInt32Sequence& indices, const CstPointSequence& normals );

    void destoryMesh(IMesh* pMesh);
    MaterialManager* getMaterialManager() const;

private:
    RenderSystem*  m_pRenderingSystem;

    std::set<IMesh*> m_meshes;
};
