#include "GLRenderer.h"
#include <SDL2/SDL.h>
#include <stack>
#include <Exception.h>
#include <sstream>


#if !defined(NDEBUG) || !defined(NGLDEBUG)
#define GLDEBUGCALL(X) X
#else
#define GLDEBUGCALL(X) X
#endif




namespace GLError
{
    std::stack<GLenum> errorCodes;
    inline void GetErrorCode()
    {
        GLenum errorCode;
        while((errorCode != glGetError()) != GL_NO_ERROR)
            errorCodes.push(errorCode);
    }
};

GLException::GLException(int line, const char* fileName, GLenum errorCode, std::string functionName)
    :
        CException(line, fileName)
{
    this->errorCode =errorCode;
    this->functionName = functionName;
}

std::string GLException::GetOriginalString()  noexcept 
{
    std::stringstream original;
    std::string errorName;
    switch(errorCode)
    {
        case GL_INVALID_OPERATION:
            errorName = "Invalid operation";
            break;
        case GL_INVALID_ENUM:
            errorName = "Invalid enum";
            break;
        case GL_INVALID_VALUE:
            errorName = "Invalid value";
            break;
    };
    original << "[ERROR CODE]: " << errorCode << std::endl
        << "[ERROR STRING]: " << errorName  << std::endl
        << "[FUNCTION NAME]: " << functionName << std::endl;
    return original.str().c_str();
}



GLIndexBinder::GLIndexBinder(uint32_t rendererID)
{
    this->rendererID = rendererID;
}

void GLIndexBinder::Bind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void GLIndexBinder::UnBind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLVertexBinder::GLVertexBinder(uint32_t rendererID)
{
    rendererID = rendererID;
}

void GLVertexBinder::Bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void GLVertexBinder::UnBind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLTextureBinder::GLTextureBinder(uint32_t rendererID)
{
    this->rendererID = rendererID;
}

GLTextureBinder::GLTextureBinder(uint32_t rendererID, GLenum target)
{
    this->rendererID = rendererID;
    this->target = target;
}

void GLTextureBinder::Bind() const noexcept
{
    glBindTexture(target, rendererID);
}

void GLTextureBinder::UnBind() const noexcept
{
    glBindTexture(target, 0);
}

GLRenderer::GLRenderer()
{
    glActiveTexture(GL_TEXTURE0);
    shaderProgram.id = glCreateProgram();
    shaderProgram.Linked = false;
} 

void GLRenderer::Uniform1f(const float& data, std::string name)
{
    glUniform1f(glGetUniformLocation(shaderProgram.id, name.c_str()), data);
}

void GLRenderer::Uniform2f(const Vect2& data, std::string name)
{
    glUniform2f(glGetUniformLocation(shaderProgram.id, name.c_str()), data.x, data.y);
}

void GLRenderer::Uniform3f(const Vect3& data, std::string name)
{
    glUniform3f(glGetUniformLocation(shaderProgram.id, name.c_str()), data.x, data.y, data.z);
}

void GLRenderer::Uniform4f(const Vect4& data, std::string name)
{
    glUniform4f(glGetUniformLocation(shaderProgram.id, name.c_str()), data.x, data.y, data.z, data.w);
}

void GLRenderer::UniformMat(const Mat& mat, std::string name)
{
    uint32_t uLocation = glGetUniformLocation(shaderProgram.id, name.c_str());
    switch(mat.dimension.column)
    {
        case 2:
            switch(mat.dimension.row)
            {
                case 2:
                    glUniformMatrix2fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 3:
                    glUniformMatrix2x3fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 4:
                    glUniformMatrix2x4fv(uLocation, mat.sizet,  GL_FALSE, mat.buffer.get());
                    break;
            }
            break;
        case 3:
            switch(mat.dimension.row)
            {
                case 2:
                    glUniformMatrix3x2fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 3:
                    glUniformMatrix3fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 4:
                    glUniformMatrix3x4fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
            }
            break;
        case 4:
            switch(mat.dimension.row)
            {
                case 2:
                    glUniformMatrix4x2fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 3:
                    glUniformMatrix4x3fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
                case 4:
                    glUniformMatrix4fv(uLocation, mat.sizet, GL_FALSE, mat.buffer.get());
                    break;
            }
            break;

    }
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
            gBinder = new GLVertexBinder(rendererID);
            break;        
        case GBuffer::GBufferStyle::BufferType::INDEX:
            bufferType = GL_ELEMENT_ARRAY_BUFFER;
            gBinder = new GLIndexBinder(rendererID);
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
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
            break;
        case Texture::Type::T2D:
            type = GL_TEXTURE_2D;
            glBindTexture(type, rendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
            break;
        case Texture::Type::T3D:
            type = GL_TEXTURE_3D;
            glBindTexture(type, rendererID);
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, texture->width, texture->height, texture->depth,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
            break;
    };
    texture->binder = new GLTextureBinder(rendererID, type);
    texture->Bind();
    glGenerateMipmap(type);
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
