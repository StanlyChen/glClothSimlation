#pragma once

#include "RenderSystem.h"
#include <vector>


class PointRenderable : public IRenderable
{
public:
    void initRenderingData(const CstPointSequence& vertices);
    void destoryRenderingData();

    void updateVertices(const CstPointSequence& vertices);

    void render(RenderSystem* pContext) override;
    void setRenderMethod(IRenderMethod* pRenderMethod) override;

private:
    IRenderMethod* m_pRenderMethod = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo_vertices = 0;
    GLuint m_ebo = 0;
    GLuint m_vertexCount = 0;

};
