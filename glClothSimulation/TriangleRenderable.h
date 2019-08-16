#pragma once

#include "RenderSystem.h"
#include <vector>


class TriangleRenderable : public IRenderable
{
public:
    void initRenderingData(const  CstPointSequence& vertices, const CstInt32Sequence& indices, const CstPointSequence& normals);
    void destoryRenderingData();
    void updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals);

    void setRenderMethod(IRenderMethod* pMethod) override;
    void render(RenderSystem* pContext) override;

private:
    IRenderMethod* m_pRenderMethod = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo_vertives = 0;
    GLuint m_vbo_normals = 0;
    GLuint m_ebo = 0;

    GLuint m_indicesCount = 0;
    GLuint m_vertexCount = 0;
};

