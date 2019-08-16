#include "RenderSystem.h"
#include <iostream>
#include <string>
#include "Material.h"

RenderSystem::RenderSystem(GLFWwindow* pWindow)
    :m_pWindow(pWindow)
{
    m_pMaterialManager = new MaterialManager();
    glfwGetFramebufferSize(m_pWindow, &m_width, &m_height);
}


RenderSystem::~RenderSystem()
{
}


void RenderSystem::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void RenderSystem::setProjMatrix(const matrix4& mat)
{
    m_projMatrix = mat;
}

void RenderSystem::setViewMatrix(const matrix4& mat)
{
    m_viewMatrix = mat;
}

MaterialManager* RenderSystem::getMaterialManager() const
{
    return m_pMaterialManager;
}


matrix4 RenderSystem::getProjMatrix()
{

    return m_projMatrix;
}

matrix4 RenderSystem::getViewMatrix()
{
    return m_viewMatrix;
}

matrix4 RenderSystem::getProjViewMatrix()
{
    return getProjMatrix()*getViewMatrix();
}


void RenderSystem::resizeGL(int w, int h)
{
    glfwGetFramebufferSize(m_pWindow, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
}


void RenderSystem::paintGL()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    for (IRenderable* pRenderable : m_renderObjects)
        pRenderable->render(this);

}


void RenderSystem::addRenderable(IRenderable* pRenderable)
{
    m_renderObjects.insert(pRenderable);
}

void RenderSystem::removeRenderable(IRenderable* pRenderable)
{
    m_renderObjects.erase(pRenderable);
}


bool ShaderUtil::compileShader( GLuint shader, const char* source, std::string& errorMsg)
{
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        errorMsg = std::string(buffer);
        return false;
    }
    else
    {
        return true;
    }
}

ShaderCompileResult ShaderUtil::buildShaderProgram(
    const char* vertexStr /*= nullptr*/, const char* fragStr/* = nullptr*/, const char* gemoStr /*= nullptr*/)
{
    ShaderCompileResult ret;
    if (!vertexStr && !fragStr && !gemoStr)
        return ret;

    ret.bSuccess = true;

    if (vertexStr)
    {
        ret.vertexShader = glCreateShader(GL_VERTEX_SHADER);
        ret.bSuccess &= compileShader( ret.vertexShader, vertexStr, ret.errorMessage);

        if (!ret.bSuccess)
            return clearFailedBuild( ret);
    }

    if (fragStr)
    {
        ret.fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        ret.bSuccess &= compileShader( ret.fragShader, fragStr, ret.errorMessage);
        if (!ret.bSuccess)
            return clearFailedBuild( ret);

    }

    if (gemoStr)
    {
        ret.geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        ret.bSuccess &= compileShader( ret.geometryShader, gemoStr, ret.errorMessage);
        if (!ret.bSuccess)
            return clearFailedBuild( ret);
    }

    ret.program = glCreateProgram();
    if (ret.vertexShader)
        glAttachShader(ret.program, ret.vertexShader);
    if (ret.geometryShader)
        glAttachShader(ret.program, ret.geometryShader);
    if (ret.fragShader)
        glAttachShader(ret.program, ret.fragShader);



    glLinkProgram(ret.program);
    GLint status;
    glGetProgramiv(ret.program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char buffer[512];
        glGetProgramInfoLog(ret.program, 512, NULL, buffer);
        ret.errorMessage = std::string(buffer);
        ret.bSuccess = false;
        return clearFailedBuild( ret);
    }

    return ret;
}

ShaderCompileResult ShaderUtil::clearFailedBuild(ShaderCompileResult& result)
{
    if (result.bSuccess)
        return result;

    if (result.program)
        glDeleteProgram(result.program);

    if (result.vertexShader)
        glDeleteShader(result.vertexShader);

    if (result.geometryShader)
        glDeleteShader(result.geometryShader);

    if (result.fragShader)
        glDeleteShader(result.fragShader);

    return result;
}


    bool RenderMethodBase::create()
    {
        const char* vertexShader = getVertexShader();
        const char* fragmentShader = getFragmentShader();
        const char* geometryShader = getGeometryShader();

        ShaderCompileResult ret = ShaderUtil::buildShaderProgram(vertexShader, fragmentShader, geometryShader);
        if (ret.bSuccess)
        {
            m_vertexShader = ret.vertexShader;
            m_fragShader = ret.fragShader;
            m_shaderProgram = ret.program;

            m_viewMatrixLoc = glGetUniformLocation(m_shaderProgram, "viewMatrix");
            m_projMatrixLoc = glGetUniformLocation(m_shaderProgram, "projMatrix");
            m_normalMatrixLoc = glGetUniformLocation(m_shaderProgram, "normalMatrix");
            return true;
        }
        else
        {
            std::cout << ret.errorMessage << std::endl;
            return false;
        }
    }

     void RenderMethodBase::destory()
     {
        glUseProgram(0);
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
        glDeleteShader(m_fragShader);
        m_fragShader = 0;
     }

     void RenderMethodBase::updateParams(RenderSystem* pContext)
     {
        if( m_viewMatrixLoc != -1)
        {
             auto viewMatrix = pContext->getViewMatrix();
             glUniformMatrix4fv(m_viewMatrixLoc, 1, false, reinterpret_cast<float*>(&viewMatrix));
        }

        if(m_projMatrixLoc != -1 )
        {
            auto projMatrix = pContext->getProjMatrix();
            glUniformMatrix4fv(m_projMatrixLoc, 1, false, reinterpret_cast<float*>(&projMatrix));
        }

        if( m_normalMatrixLoc != -1)
        {
            auto viewMatrix = pContext->getViewMatrix();
            auto normalMatrix = glm::transpose(glm::inverse(viewMatrix));
            glUniformMatrix4fv(m_normalMatrixLoc, 1, false, reinterpret_cast<float*>(&normalMatrix));
        }

     }

     void RenderMethodBase::bind()
     {
        glUseProgram(m_shaderProgram);
     }

     void RenderMethodBase::unbind()
     {
        glUseProgram(0);
     }

     const char* RenderMethodBase::getVertexShader()
     {
        return nullptr;
     }

     const char* RenderMethodBase::getGeometryShader()
     {
        return nullptr;
     }

     const char* RenderMethodBase::getFragmentShader()
     {
        return nullptr;
     }