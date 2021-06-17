#include "GLRenderer.h"
#include <SDL2/SDL.h>
#include <stack>
#include <Exception.h>
namespace GLError
{
    static std::stack<GLenum> erroCodes;
    static void GetErrorCode();
};

#ifndef NDEBUG
#ifndef NGLDEBUG
#define GLDEBUGCALL(x)
#endif
#endif



void GLIndexBinder::Bind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void GLIndexBinder::UnBind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLVertexBinder::Bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void GLVertexBinder::UnBind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


GLRenderer::GLRenderer()
{
    shaderProgram.id = glCreateProgram();
    shaderProgram.Linked = false;
} 

void GLRenderer::FinalizeVertexSpecification()
{
    uint32_t totalSize = 0;
    for(auto i: specification) totalSize += i.second;
    uint32_t offset = 0;
    totalSize *= sizeof(float);
    for(uint32_t i = 0; i < specification.size(); i++)
    {
        glBindAttribLocation(shaderProgram.id, i, specification[i].first.c_str());
        glVertexAttribPointer(i, specification[i].second, GL_FLOAT, GL_FALSE, 
                totalSize, (const void*)offset);
        offset+= specification[i].second * sizeof(float);
        glEnableVertexAttribArray(i);
    }
}

void GLRenderer::UseCurrentShaderProgram()
{
    if(!shaderProgram.Linked)
    {
        glLinkProgram(shaderProgram.id);
        int32_t programLinked;
        glGetProgramiv(shaderProgram.id, GL_LINK_STATUS, &programLinked);
        if(programLinked != GL_TRUE)
        {
            char message[1024];
            int32_t logLength;
            glGetProgramInfoLog(programLinked, 1024, &logLength, message);
            throw CException(__LINE__, __FILE__, "Program Linking Error", message);
        }
        shaderProgram.Linked = true;
        for(uint32_t i = 0; i < shaders.size();i++)
            glDeleteShader(shaders[i]);
    }
    glUseProgram(shaderProgram.id);
}


void GLRenderer::LoadBuffer(GBuffer* gBuffer)
{
    GLenum flags;
    GLenum bufferType;
    GBinder* gBinder;
    uint32_t rendererID;
    switch(gBuffer->bufferStyle.cpuFlags)
    {
        case GBuffer::GBufferStyle::CPUFlags::STATIC:
            switch(gBuffer->bufferStyle.usage)
            {
                case GBuffer::GBufferStyle::Usage::DRAW:
                    flags = GL_STATIC_DRAW;
                    break;
                case GBuffer::GBufferStyle::Usage::READ:
                    flags = GL_STATIC_READ;
                    break;
                case GBuffer::GBufferStyle::Usage::COPY:
                    flags = GL_STATIC_COPY;
                    break;
            };
            break;
        case GBuffer::GBufferStyle::CPUFlags::DYNAMIC:
            switch(gBuffer->bufferStyle.usage)
            {
                case GBuffer::GBufferStyle::Usage::DRAW:
                    flags = GL_DYNAMIC_DRAW;
                    break;
                case GBuffer::GBufferStyle::Usage::READ:
                    flags = GL_DYNAMIC_READ;
                    break;
                case GBuffer::GBufferStyle::Usage::COPY:
                    flags = GL_DYNAMIC_COPY;
                    break;
            };
            break;        
        case GBuffer::GBufferStyle::CPUFlags::STREAM:
            switch(gBuffer->bufferStyle.usage)
            {
                case GBuffer::GBufferStyle::Usage::DRAW:
                    flags = GL_STREAM_DRAW;
                    break;
                case GBuffer::GBufferStyle::Usage::READ:
                    flags = GL_STREAM_READ;
                    break;
                case GBuffer::GBufferStyle::Usage::COPY:
                    flags = GL_STREAM_COPY;
                    break;
            };
            break;
    }
    glGenBuffers(gBuffer->count, &rendererID);
    switch(gBuffer->bufferStyle.type)
    {
        case GBuffer::GBufferStyle::BufferType::VERTEX:
            bufferType = GL_ARRAY_BUFFER;
            gBinder = new GLVertexBinder();
            static_cast<GLVertexBinder*>(gBinder)->rendererID = rendererID;
            break;        
        case GBuffer::GBufferStyle::BufferType::INDEX:
            bufferType = GL_ELEMENT_ARRAY_BUFFER;
            gBinder = new GLIndexBinder();
            static_cast<GLIndexBinder*>(gBinder)->rendererID = rendererID;
            break;
    };
    
    gBuffer->Bind();
    glBufferData(bufferType, gBuffer->sizet, gBuffer->data.get(), flags);
}

void GLRenderer::LoadTexture(Texture* texture)
{
    uint32_t rendererID;
    glGenTextures(1, &rendererID);
    GLenum type;
    switch(texture->type)
    {
        case Texture::Type::T1D:
            type = GL_TEXTURE_1D;
            glBindTexture(type, rendererID);
            glTexImage1D(type, 0, GL_RGBA, texture->width, 
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data.get());
            break;
        case Texture::Type::T2D:
            type = GL_TEXTURE_2D;
            glBindTexture(type, rendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data.get());
            break;
        case Texture::Type::T3D:
            type = GL_TEXTURE_3D;
            glBindTexture(type, rendererID);
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, texture->width, texture->height, texture->depth,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data.get());
            break;
    };
}

void GLRenderer::AddShader(ShaderType type, std::string& source)
{
    GLenum shaderType;
    switch(type)
    {
        case ShaderType::VERTEX:
            shaderType = GL_VERTEX_SHADER;
            break;
        case ShaderType::FRAGMENT:
            shaderType = GL_FRAGMENT_SHADER;
            break;
    };
    uint32_t shader = glCreateShader(shaderType);
    const char* shaderSource =source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    int32_t shaderCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
    if(shaderCompiled != GL_TRUE)
    {
        char message[1024];
        int32_t logLength;
        glGetShaderInfoLog(shaderCompiled, 1024, &logLength, message);
        throw CException(__LINE__, __FILE__, "Shader Compile Error", message);
    }
    shaders.push_back(shader);
    glAttachShader(shaderProgram.id, shader);
}
void GLRenderer::Draw(GBuffer* gBuffer)
{
    glDrawElements(GL_TRIANGLES, gBuffer->count, GL_UNSIGNED_INT, (const void*) 0);
}

void GLRenderer::Clear()
{
    glClear(GL_COLOR | GL_DEPTH);
}


void GLRenderer::LoadGladGL()
{
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        if(!gladLoadGL())
        {
        }
    }
}
