#pragma once

#include "RenderSystem.h"


class LineRenderable : public IRenderable
{
public:
    void initRenderingData(const  CstPointSequence& vertices, const CstInt32Sequence& indices);
    void destoryRenderingData();

    void updateVertices(const CstPointSequence& vertices);

    void setRenderMethod(IRenderMethod* pMethod) override;
    void render(RenderSystem* pContext) override;

private:
    IRenderMethod* m_pRenderMethod = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo_vertives = 0;
    GLuint m_ebo = 0;

    GLuint m_indicesCount = 0;
    GLuint m_vertexCount = 0;
};

