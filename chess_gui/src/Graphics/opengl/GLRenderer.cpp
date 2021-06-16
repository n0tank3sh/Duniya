#include "GLRenderer.h"
#include <SDL2/SDL.h>

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
    glBufferData(bufferType, gBuffer->count, gBuffer->data.get(), flags);
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
