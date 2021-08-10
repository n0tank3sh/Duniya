#include "GLRenderer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <Exception.h>
#include <sstream>


#if !defined(NDEBUG) || !defined(NGLDEBUG)
#define GLDEBUGCALL(X) GLError::GetErrorCode();GLError::errorCodes.clear(); X; GLError::GetErrorCode(); if(!GLError::errorCodes.empty()) throw GLException(__LINE__ , __FILE__, GLError::errorCodes.back(), #X);
#else
#define GLDEBUGCALL(X) X
#endif




namespace GLError
{
    std::vector<GLenum> errorCodes;
    inline void GetErrorCode()
    {
        GLenum errorCode;
        while((errorCode = glGetError()) != GL_NO_ERROR)
            errorCodes.push_back(errorCode);
    }
};

GLException::GLException(int line, const char* fileName, GLenum errorCode, std::string functionName)
    :
        CException(line, fileName)
{
    this->errorCode =errorCode;
    this->functionName = functionName;
	type = "GLException";
}

const char* GLException::what() const noexcept
{
	GetOriginalString();
	std::ostringstream oss(whatBuffer);
	oss << "[FUNCTION] :" << functionName << "\n"
		<< "[ERROR CODE] :" << errorCode << "\n"
		<< "[ERROR] :" << CheckError() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string GLException::CheckError() const noexcept 
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
		default:
			errorName = "Unknown Error";
    };
    original << "[ERROR CODE]: " << errorCode << std::endl
        << "[ERROR STRING]: " << errorName  << std::endl
        << "[FUNCTION NAME]: " << functionName << std::endl;
    return original.str();
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
    this->rendererID = rendererID;
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
    LoadGladGL();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    GLDEBUGCALL(glGenVertexArrays(1, &pvao));
    glBindVertexArray(pvao);
    GLDEBUGCALL(glActiveTexture(GL_TEXTURE0));
    GLDEBUGCALL(shaderProgram.id = glCreateProgram());
    shaderProgram.Linked = false;
} 


void GLRenderer::Uniform1f(const uint32_t count, const float* data, std::string name)
{
	if(count == 1)
    	glUniform1f(glGetUniformLocation(shaderProgram.id, name.c_str()), *data);
	else
	{
		glUniform1fv(glGetUniformLocation(shaderProgram.id, name.c_str()), count,  data);
	}
}

void GLRenderer::Uniform1u(const uint32_t count, const uint32_t* data, std::string name)
{
	if(count == 1)
		glUniform1ui(glGetUniformLocation(shaderProgram.id, name.c_str()), *data);
	else
	{
		glUniform1uiv(glGetUniformLocation(shaderProgram.id, name.c_str()), count, data);
	}
}

void GLRenderer::Uniform1i(const uint32_t count, const int32_t* data, std::string name)
{
	if(count == 1)
		glUniform1i(glGetUniformLocation(shaderProgram.id, name.c_str()), *data);
	else
	{
		glUniform1iv(glGetUniformLocation(shaderProgram.id, name.c_str()), count, data);
	}
}

void GLRenderer::Uniform2f(const uint32_t count, const Vect2* data, std::string name)
{
	if(count == 1)
    	glUniform2f(glGetUniformLocation(shaderProgram.id, name.c_str()), data->x, data->y);
	else
	{
		glUniform2fv(glGetUniformLocation(shaderProgram.id, name.c_str()), count, (float*)data);
	}
	
}

void GLRenderer::Uniform3f(const uint32_t count, const Vect3* data, std::string name)
{
	if(count == 1)
    	glUniform3f(glGetUniformLocation(shaderProgram.id, name.c_str()), data->x, data->y, data->z);
	else
	{
	}
}

void GLRenderer::Uniform4f(const uint32_t count, const Vect4* data, std::string name)
{
	if(count == 1)
		glUniform4f(glGetUniformLocation(shaderProgram.id, name.c_str()), data->x, data->y, data->z, data->w);
	else
	{
		glUniform4fv(glGetUniformLocation(shaderProgram.id, name.c_str()), count,  (float*)data);
	}
}

void GLRenderer::UniformMat(const uint32_t count, const Mat* mat, std::string name)
{
	UseCurrentShaderProgram();
    GLDEBUGCALL(uint32_t uLocation = glGetUniformLocation(shaderProgram.id, name.c_str()));
	float* answer = new float[mat->sizet * count];
	for(uint32_t i = 0; i < count; i++)
	{
		std::copy(mat[i].buffer.get(), mat[i].buffer.get() + mat[i].sizet, answer + (i * mat->sizet * sizeof(float)));
	}
    switch(mat->dimension.column)
    {
        case 2:
            switch(mat->dimension.row)
            {
                case 2:
                    glUniformMatrix2fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 3:
                    glUniformMatrix2x3fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 4:
                    glUniformMatrix2x4fv(uLocation, count,  GL_FALSE, answer);
                    break;
            }
            break;
        case 3:
            switch(mat->dimension.row)
            {
                case 2:
                    glUniformMatrix3x2fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 3:
                    glUniformMatrix3fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 4:
                    glUniformMatrix3x4fv(uLocation, count, GL_FALSE, answer);
                    break;
            }
            break;
        case 4:
            switch(mat->dimension.row)
            {
                case 2:
                    glUniformMatrix4x2fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 3:
                    glUniformMatrix4x3fv(uLocation, count, GL_FALSE, answer);
                    break;
                case 4:
                    GLDEBUGCALL(glUniformMatrix4fv(uLocation, count, GL_FALSE, answer));
                    break;
            }
            break;

    }
}

void GLRenderer::ClearColor(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

void GLRenderer::ClearDepth(float depthLevel)
{
    glClearDepth(depthLevel);
}

void GLRenderer::FinalizeVertexSpecification()
{
	glBindVertexArray(pvao);
	UseCurrentShaderProgram();
    uint32_t totalSize = 0;
    for(auto& i: specification) totalSize += i.second;
    uint32_t offset = 0;
    totalSize *= sizeof(float);
    for(uint32_t i = 0; i < specification.size(); i++)
    {
        GLDEBUGCALL(glBindAttribLocation(shaderProgram.id, i, specification[i].first.c_str()));
        GLDEBUGCALL(glVertexAttribPointer(i, specification[i].second, GL_FLOAT, GL_FALSE, 
                totalSize, (const void*)offset));
        GLDEBUGCALL(glEnableVertexAttribArray(i));
        offset+= specification[i].second * sizeof(float);
    }
}

void GLRenderer::UseCurrentShaderProgram()
{
    if(!shaderProgram.Linked)
    {
        GLDEBUGCALL(glLinkProgram(shaderProgram.id));
        int32_t programLinked;
        GLDEBUGCALL(glGetProgramiv(shaderProgram.id, GL_LINK_STATUS, &programLinked));
        if(programLinked != GL_TRUE)
        {
            char message[1024];
            int32_t logLength;
            glGetProgramInfoLog(programLinked, 1024, &logLength, message);
            throw CException(__LINE__, __FILE__, "Program Linking Error", message);
        }
        for(uint32_t i = 0; i < shaders.size();i++)
            glDeleteShader(shaders[i]);
		shaderProgram.Linked = true;
    }
    GLDEBUGCALL(glUseProgram(shaderProgram.id));
}


void GLRenderer::LoadBuffer(GBuffer* gBuffer)
{
	glBindVertexArray(pvao);
    GLenum flags;
    GLenum bufferType;
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
    glGenBuffers(1, &rendererID);
    switch(gBuffer->bufferStyle.type)
    {
        case GBuffer::GBufferStyle::BufferType::VERTEX:
            bufferType = GL_ARRAY_BUFFER;
            gBuffer->gBinder.reset(new GLVertexBinder(rendererID));
			gBuffer->Bind();
            FinalizeVertexSpecification();
            break;        
        case GBuffer::GBufferStyle::BufferType::INDEX:
            bufferType = GL_ELEMENT_ARRAY_BUFFER;
            gBuffer->gBinder.reset(new GLIndexBinder(rendererID));
            break;
    };

    GLDEBUGCALL(gBuffer->Bind());
    GLDEBUGCALL(glBufferData(bufferType, gBuffer->sizet, gBuffer->data, flags));
}

void GLRenderer::LoadTexture(Texture* texture, GBuffer* gBuffer)
{
    uint32_t rendererID;
	gBuffer = new GBuffer;
    if(texture == nullptr) std::cout << "Texture is nullptr " << std::endl;
    GLDEBUGCALL(glGenTextures(1, &rendererID));
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
            GLDEBUGCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data));
            break;
        case Texture::Type::T3D:
            type = GL_TEXTURE_3D;
            glBindTexture(type, rendererID);
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, texture->width, texture->height, texture->depth,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
            break;
    };
    gBuffer->gBinder = std::unique_ptr<GBinder>(new GLTextureBinder(rendererID, type));
    gBuffer->Bind();
    GLDEBUGCALL(glGenerateMipmap(type));
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
    GLDEBUGCALL(uint32_t shader = glCreateShader(shaderType));
    const char* shaderSource =source.c_str();
    GLDEBUGCALL(glShaderSource(shader, 1, &shaderSource, nullptr));
    GLDEBUGCALL(glCompileShader(shader));
    int32_t shaderCompiled;
    GLDEBUGCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled));
    if(shaderCompiled != GL_TRUE)
    {
        char message[1024];
		int32_t logLength;
        GLDEBUGCALL(glGetShaderInfoLog(shader, 1024, &logLength, message));
        throw CException(__LINE__, __FILE__, "Shader Compile Error", std::string(message) + (shaderType == GL_VERTEX_SHADER?"Vertex Shader":"Fragment Shader"));
    }
    shaders.push_back(shader);
    GLDEBUGCALL(glAttachShader(shaderProgram.id, shader));
}
void GLRenderer::Draw(GBuffer* gBuffer)
{
	gBuffer->Bind();
    GLDEBUGCALL(glDrawElements(GL_TRIANGLES, gBuffer->count, GL_UNSIGNED_INT, (const void*) 0));
}

void GLRenderer::Clear()
{
    GLDEBUGCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


void GLRenderer::LoadGladGL()
{
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        if(!gladLoadGL())
        {
            throw CException(__LINE__, __FILE__, "glad Load error", "Can't load glad");
        }
    }
}
