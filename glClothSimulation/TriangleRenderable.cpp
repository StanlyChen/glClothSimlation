#include "TriangleRenderable.h"
#include <iostream>
#include <string>

void TriangleRenderable::initRenderingData(const CstPointSequence& vertices, const CstInt32Sequence& indices, const CstPointSequence& normals)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo_vertives);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertives);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vect3)*vertices.size(), vertices.r_address(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    if (normals.empty() == false)
    {
        glGenBuffers(1, &m_vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vect3)*normals.size(), normals.r_address(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }


    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.r_address(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_vertexCount = (unsigned)vertices.size();
    m_indicesCount = (unsigned)indices.size();

    m_pRenderMethod = nullptr;

}

void TriangleRenderable::setRenderMethod(IRenderMethod* pMethod) 
{
    m_pRenderMethod = pMethod;
}

void TriangleRenderable::destoryRenderingData()
{
    glDeleteVertexArrays(1, &m_vao);
    if(m_vbo_normals)
        glDeleteBuffers(1, &m_vbo_normals);
    glDeleteBuffers(1, &m_vbo_vertives);
    glDeleteBuffers(1, &m_ebo);

    m_pRenderMethod = nullptr;

}

void TriangleRenderable::updateVertices(const CstPointSequence& vertices, const CstPointSequence& normals)
{
    if (m_vbo_vertives != -1 && vertices.size() == m_vertexCount)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertives);

        vect3* pTarget = (vect3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(pTarget, vertices.r_address(), sizeof(vect3)*vertices.size());

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (m_vbo_normals != -1 && normals.size() == m_vertexCount)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);

        vect3* pTarget = (vect3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(pTarget, normals.r_address(), sizeof(vect3)*normals.size());

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


void TriangleRenderable::render(RenderSystem* pContext)
{
    if (!m_pRenderMethod)
        return;

    glBindVertexArray(m_vao);

    m_pRenderMethod->bind();
    m_pRenderMethod->updateParams(pContext);

    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);

    m_pRenderMethod->unbind();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



