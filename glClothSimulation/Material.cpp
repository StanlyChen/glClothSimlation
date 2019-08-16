#include "Material.h"
#include <iostream>
#include <string>

bool SimplePointRenderMethod::create()
{
    const char* Point_VS = R"(

#version 450 core

uniform mat4x4 viewMatrix;
uniform mat4x4 projMatrix;

layout(location =0) in vec3 position;
void main()
{
	gl_Position = projMatrix*viewMatrix*vec4(position, 1.0);
}

)";

    const char* Point_FS = R"(

#version 450 core
layout( location  = 0) out vec4 outColor;
uniform vec3 color = vec3(1,1,0);

void main()
{
    outColor = vec4( color, 1 );
}

)";

    const char* geomtryShaderStr = nullptr;

    ShaderCompileResult ret = ShaderUtil::buildShaderProgram(Point_VS, Point_FS, geomtryShaderStr);
    if (ret.bSuccess)
    {
        m_vertexShader = ret.vertexShader;
        m_fragShader = ret.fragShader;
        m_shaderProgram = ret.program;

        m_viewMatrixLoc = glGetUniformLocation(m_shaderProgram, "viewMatrix");
        m_projMatrixLoc = glGetUniformLocation(m_shaderProgram, "projMatrix");
        //m_pointSizeLoc = glGetUniformLocation(m_shaderProgram, "pointSize");
        m_colorLoc = glGetUniformLocation(m_shaderProgram, "color");

        return true;
    }
    else
    {
        std::cout << ret.errorMessage << std::endl;
        return false;
    }
}

void SimplePointRenderMethod::destory()
{
    glUseProgram(0);
    glDeleteProgram(m_shaderProgram);
    m_shaderProgram = 0;
    glDeleteShader(m_vertexShader);
    m_vertexShader = 0;
    glDeleteShader(m_fragShader);
    m_fragShader = 0;
}


void SimplePointRenderMethod::bind()
{
    glUseProgram(m_shaderProgram);
}

void SimplePointRenderMethod::unbind()
{
    glUseProgram(0);
}


void SimplePointRenderMethod::updateParams(RenderSystem* pContext)
{
    if (m_viewMatrixLoc == -1
        || m_projMatrixLoc == -1)
        return;

    auto viewMatrix = pContext->getViewMatrix();
    glUniformMatrix4fv(m_viewMatrixLoc, 1, false, reinterpret_cast<float*>(&viewMatrix));

    auto projMatrix = pContext->getProjMatrix();
    glUniformMatrix4fv(m_projMatrixLoc, 1, false, reinterpret_cast<float*>(&projMatrix));

    //glUniform1f(m_pointSizeLoc, m_pointSize);
    glUniform3f(m_colorLoc, m_color.r, m_color.g, m_color.b);
}

bool SimpleLineRenderMethod::create()
{
    bool ret = RenderMethodBase::create();
    if (ret)
    {
        m_colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    }
    return ret;
}

void SimpleLineRenderMethod::updateParams(RenderSystem* pContext)
{
    RenderMethodBase::updateParams(pContext);
    if (m_colorLoc != -1)
    {
        glUniform3f(m_colorLoc, m_color.r, m_color.g, m_color.b);
    }
}

const char* SimpleLineRenderMethod::getVertexShader()
{
    const char* LINE_VS = R"(
#version 450 core

uniform mat4x4 viewMatrix;
uniform mat4x4 projMatrix;

layout(location =0) in vec3 position;
void main()
{
	gl_Position = projMatrix*viewMatrix*vec4(position, 1.0);
}

)";
    return LINE_VS;
}

const char* SimpleLineRenderMethod::getFragmentShader()
{
    const char* LINE_FS = R"(

#version 450 core
layout( location  = 0) out vec4 outColor;
uniform vec3 color = vec3(0,0,0);

void main()
{
    outColor = vec4( color, 1 );
}

)";
    return LINE_FS;
}

const char* SimpleTriangleRenderMethod::getVertexShader()
{
    const char* TRIANGLE_VS = R"(

#version 450 core

uniform mat4x4 viewMatrix;
uniform mat4x4 projMatrix;

layout(location =0) in vec3 position;
layout(location =1) in vec3 normals;
void main()
{
	gl_Position = projMatrix*viewMatrix*vec4(position, 1.0);
}

)";
    return TRIANGLE_VS;
}

const char* SimpleTriangleRenderMethod::getFragmentShader()
{
    const char* TRIANGLE_FS = R"(

#version 450 core
layout( location  = 0) out vec4 outColor;
uniform vec3 color = vec3(1,1,0);

void main()
{
    outColor = vec4( color, 1 );
}

)";
    return TRIANGLE_FS;
}


void SimpleTriangleRenderMethod::updateParams(RenderSystem* pContext)
{
    RenderMethodBase::updateParams(pContext);
    if (m_colorLoc != -1)
    {
        glUniform3f(m_colorLoc, m_color.r, m_color.g, m_color.b);
    }
}

//--------------------------------------------------------------------------
const char* ShadedTriangleRenderMethod::getVertexShader()
{
    const char* TRIANGLE_VS = R"(

    #version 450 core

    uniform mat4x4 viewMatrix;
    uniform mat4x4 projMatrix;
    uniform mat4x4 normalMatrix;
    uniform vec3 color = vec3(1,1,1);

    layout(location =0) in vec3 iPosition;
    layout(location =1) in vec3 iNormal;

    layout(location=0) out vec3 oNormal;
    layout(location=1) out vec3 oColor;
    void main()
    {
	    gl_Position = projMatrix*viewMatrix*vec4(iPosition, 1.0);
        oNormal =  normalize( (normalMatrix*vec4(iNormal, 0.0)).xyz);
        oColor = color;

    }

    )";

    return TRIANGLE_VS;
}

const char* ShadedTriangleRenderMethod::getFragmentShader()
{
    const char* TRIANGLE_FS = R"(

#version 450 core
layout( location  =0) in vec3 iNormal;
layout( location  =1) in vec3 iColor;
layout( location  =0) out vec4 outColor;

void main()
{
    vec3 lightDirection = vec3(0,0,1);
    float diffuse = abs( dot( lightDirection, iNormal));
    outColor = vec4(diffuse*iColor,1);
}

)";
    return TRIANGLE_FS;
}


const char* WireFrameTriangleRenderMethod::getVertexShader()
{
    const char* TRIANGLE_VS = R"(

    #version 450 core

    uniform mat4x4 viewMatrix;
    uniform mat4x4 projMatrix;

    layout(location =0) in vec3 iPosition;

    void main()
    {
	    gl_Position = projMatrix*viewMatrix*vec4(iPosition, 1.0);
    }

    )";

    return TRIANGLE_VS;
}

const char* WireFrameTriangleRenderMethod::getGeometryShader()
{
    // input_primitive: points, lines, lines_adjacency, triangles, triangles_adjacency
    // output_primitive:points, line_strip, triangle_strip
    const char* TRIANGLE_GS = R"(
    #version 450 core

    layout( triangles ) in;
    layout( line_strip , max_vertices = 4) out;

    void main()
    {
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        gl_Position = gl_in[1].gl_Position;
        EmitVertex();
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();

        EndPrimitive();
    }

)";
    return TRIANGLE_GS;
}

const char* WireFrameTriangleRenderMethod::getFragmentShader()
{
    const char* FS = R"(
        #version 450 core

        layout(location = 0) out vec4 outColor;
        uniform vec3 color;

        void main()
        {
            outColor = vec4(color,1);
        }

        )";

    return FS;
}

WireFrameTriangleRenderMethod::WireFrameTriangleRenderMethod(const vect3& color)
    :m_color(color)
{}

bool WireFrameTriangleRenderMethod::create()
{
    bool ret = RenderMethodBase::create();
    if (ret)
    {
        m_colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    }
    return ret;
}

void WireFrameTriangleRenderMethod::updateParams(RenderSystem* pContext)
{
    RenderMethodBase::updateParams(pContext);
    if (m_colorLoc != -1)
    {
        glUniform3f(m_colorLoc, m_color.r, m_color.g, m_color.b);
    }
}


const char* ClothTriangleRenderMethod::getVertexShader()
{
    const char* TRIANGLE_VS = R"(

    #version 450 core

    uniform mat4x4 viewMatrix;
    uniform mat4x4 projMatrix;
    uniform mat4x4 normalMatrix;
    uniform vec3 color = vec3(1,1,1);
    uniform vec3 lightDirection = vec3(0,0,1);


    layout(location =0) in vec3 iPosition;
    layout(location =1) in vec3 iNormal;
    
    layout(location=1) out vec3 oColor;
    void main()
    {
	    gl_Position = projMatrix*viewMatrix*vec4(iPosition, 1.0);
        vec3 normal =  normalize( (normalMatrix*vec4(iNormal, 0.0)).xyz);
        float diffuse = abs( dot( lightDirection, normal));
        oColor = diffuse*color;

    }

    )";

    return TRIANGLE_VS;
}

const char* ClothTriangleRenderMethod::getFragmentShader()
{
    const char* TRIANGLE_FS = R"(

#version 450 core

layout( location  =1) in vec3 iColor;
layout( location  =0) out vec4 outColor;

void main()
{
    outColor = vec4(iColor,1);
}

)";
    return TRIANGLE_FS;
}

void ClothTriangleRenderMethod::bind()
{
    RenderMethodBase::bind();

    glDisable(GL_CULL_FACE);

}

void ClothTriangleRenderMethod::unbind()
{
    RenderMethodBase::unbind();

    glEnable(GL_CULL_FACE);
}

//---------------------------------------------

SimplePointRenderMethod*  MaterialManager::getSimplePointRenderMethod()
{
    SimplePointRenderMethod* pMethod = new SimplePointRenderMethod();
    pMethod->create();
    return pMethod;
}

SimpleLineRenderMethod* MaterialManager::getSimpleLineRenderMethod()
{
    SimpleLineRenderMethod* pMethod = new SimpleLineRenderMethod();
    pMethod->create();
    return pMethod;
}

SimpleTriangleRenderMethod* MaterialManager::getSimpleTriangleRenderMethod()
{
    SimpleTriangleRenderMethod* pMethod = new SimpleTriangleRenderMethod();
    pMethod->create();
    return pMethod;
}

ShadedTriangleRenderMethod* MaterialManager::getShadedTriangleRenderMethod()
{
    ShadedTriangleRenderMethod* pMethod = new ShadedTriangleRenderMethod();
    pMethod->create();
    return pMethod;
}

WireFrameTriangleRenderMethod* MaterialManager::getWireFrameTriangleRenderMethod(const vect3 color )
{
    WireFrameTriangleRenderMethod* pMethod = new WireFrameTriangleRenderMethod(color);
    pMethod->create();
    return pMethod;
}


ClothTriangleRenderMethod* MaterialManager::getClothTriangleRenderMethod()
{
    ClothTriangleRenderMethod* pMethod = new ClothTriangleRenderMethod();
    pMethod->create();
    return pMethod;
}