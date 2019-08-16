#pragma once

#include "RenderSystem.h"

class SimplePointRenderMethod : public IRenderMethod
{
public:
    bool create() override;
    void destory() override;
    void bind() override;
    void unbind() override;

    void updateParams(RenderSystem* pContext);

private:
    float m_pointSize = 5;
    vect3 m_color = { 1,1,0 };

    GLuint m_vertexShader = 0;
    GLuint m_fragShader = 0;
    GLuint m_shaderProgram = 0;

    GLint m_projMatrixLoc = -1;
    GLint m_viewMatrixLoc = -1;
    GLint m_pointSizeLoc = -1;
    GLint m_colorLoc = -1;
};

class SimpleLineRenderMethod : public RenderMethodBase
{
public:
    const char* getVertexShader() override;
    const char* getFragmentShader() override;

    bool create() override;
    void updateParams(RenderSystem* pContext) override;

private:
    GLuint m_colorLoc = -1;
    vect3 m_color = { 1,1,1 };
};

class SimpleTriangleRenderMethod : public RenderMethodBase
{
public:
    const char* getVertexShader() override;
    const char* getFragmentShader() override;


    void updateParams(RenderSystem* pContext) override;

private:
    GLuint m_colorLoc = -1;
    vect3 m_color = { 1,1,0 };
};

class ShadedTriangleRenderMethod : public RenderMethodBase
{
public:
    const char* getVertexShader() override;
    const char* getFragmentShader()override;
};

class WireFrameTriangleRenderMethod : public RenderMethodBase
{
public:
    WireFrameTriangleRenderMethod(const vect3& color);

public:
    const char* getVertexShader() override;
    const char* getFragmentShader()override;
    const char* getGeometryShader() override;
    
    bool create() override;
    void updateParams(RenderSystem* pContext) override;

private:
    GLuint m_colorLoc = -1;
    vect3 m_color = { 1,1,1 };
};

class ClothTriangleRenderMethod : public RenderMethodBase
{
public:
    const char* getVertexShader() override;
    const char* getFragmentShader()override;

    void bind() override;
    void unbind() override;
};

class MaterialManager
{
public:
    SimplePointRenderMethod*    getSimplePointRenderMethod();
    SimpleLineRenderMethod*     getSimpleLineRenderMethod();
    SimpleTriangleRenderMethod* getSimpleTriangleRenderMethod();
    ShadedTriangleRenderMethod* getShadedTriangleRenderMethod();
    WireFrameTriangleRenderMethod* getWireFrameTriangleRenderMethod(const vect3 color = { 1,1,1 });
    ClothTriangleRenderMethod* getClothTriangleRenderMethod();
};


