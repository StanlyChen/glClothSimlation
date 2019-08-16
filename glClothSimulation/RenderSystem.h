#pragma once
#include <set>
#include "RenderingTypes.h"
#include <functional>
#include "glHead.h"

struct GLFWWindow;
class IRenderable;
class IRenderMethod;
class MaterialManager;

class RenderSystem 
{
public:
    RenderSystem(GLFWwindow* pWindow);
    ~RenderSystem();

    void initializeGL() ;
    void resizeGL(int w, int h) ;
    void paintGL() ;


    void addRenderable(IRenderable* pRenderable);
    void removeRenderable(IRenderable* pRenderable);

    matrix4 getProjMatrix();
    matrix4 getViewMatrix();
    matrix4 getProjViewMatrix();

    void setProjMatrix(const matrix4& mat);
    void setViewMatrix(const matrix4& mat);

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }


    MaterialManager* getMaterialManager() const;

private:
     MaterialManager*      m_pMaterialManager;
     GLFWwindow*            m_pWindow;
     std::set<IRenderable*> m_renderObjects;
     matrix4 m_viewMatrix;
     matrix4 m_projMatrix;
     int m_width;
     int m_height;
};

class IRenderable 
{
public:
    virtual void setRenderMethod(IRenderMethod* pRenderMethod) = 0;
    virtual void render(RenderSystem* pContext) = 0;
};


struct ShaderCompileResult
{
    bool bSuccess = false;
    std::string errorMessage;
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
};

class ShaderUtil
{
public:
    static  ShaderCompileResult buildShaderProgram(const char* vertexStr = nullptr, const char* fragStr = nullptr, const char* gemoStr = nullptr);
    static  ShaderCompileResult clearFailedBuild(ShaderCompileResult& result);
    static bool compileShader(GLuint shader, const char* source, std::string& errorMsg);
};

class IRenderMethod
{
public:
    virtual ~IRenderMethod() {};
    virtual void reload() {};
    virtual bool create() { return false; };
    virtual void destory() {};
    virtual void updateParams(RenderSystem*) {};
    virtual void bind() {};
    virtual void unbind() {};
};


class RenderMethodBase : public IRenderMethod
{
public:
     bool create() override;
     void destory() override;
     void updateParams(RenderSystem*) override;
     void bind() override;
     void unbind() override;

     virtual const char* getVertexShader();
     virtual const char* getGeometryShader();
     virtual const char* getFragmentShader();

protected:
    GLuint m_vertexShader = 0;
    GLuint m_fragShader = 0;
    GLuint m_geometryShader = 0;
    GLuint m_shaderProgram = 0;

    GLint m_projMatrixLoc = -1;
    GLint m_viewMatrixLoc = -1;
    GLint m_normalMatrixLoc = -1;
};

