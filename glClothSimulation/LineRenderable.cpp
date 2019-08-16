#include "LineRenderable.h"


void LineRenderable::initRenderingData(const CstPointSequence& vertices, const CstInt32Sequence& indices)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo_vertives);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertives);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vect3)*vertices.size(), vertices.r_address(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.r_address(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_vertexCount = (unsigned)vertices.size();
    m_indicesCount = (unsigned)indices.size();
    m_pRenderMethod = nullptr;
}

void LineRenderable::destoryRenderingData()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertives);
    glDeleteBuffers(1, &m_ebo);
    m_pRenderMethod = nullptr;
}

void LineRenderable::updateVertices(const CstPointSequence& vertices)
{
    if (m_vbo_vertives != -1 && vertices.size() == m_vertexCount)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertives);

        vect3* pTarget = (vect3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(pTarget, vertices.r_address(), sizeof(vect3)*vertices.size());

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


void LineRenderable::setRenderMethod(IRenderMethod* pMethod)
{
    m_pRenderMethod = pMethod;
}

void LineRenderable::render(RenderSystem* pContext)
{
    if (!m_pRenderMethod)
        return;

    glBindVertexArray(m_vao);

    m_pRenderMethod->bind();
    m_pRenderMethod->updateParams(pContext);

    glDrawElements(GL_LINES, m_indicesCount, GL_UNSIGNED_INT, 0);

    m_pRenderMethod->unbind();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
