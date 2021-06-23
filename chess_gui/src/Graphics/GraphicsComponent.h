#pragma once
#include <math/vect2.h>
#include <math/vect3.h>
#include <math/vect4.h>
#include "Renderer.h"

struct Vertex
{
    Vect4 aPos;
    Vect3 aNormal;
    Vect3 texCord;
};

struct Mesh
{
    std::vector<Vertex> verticies;
    std::vector<uint32_t> indicies;
    GBuffer* vertexBuffer;
    GBuffer* indexBuffer;
    Texture* texture;
};

static void CreateGBufferMesh(Mesh* mesh)
{
    GBuffer* vertexBuffer =  new GBuffer;
    GBuffer* indexBuffer = new GBuffer;
    vertexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC; 
    vertexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::VERTEX;
    vertexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    vertexBuffer->data = mesh->verticies.data();
    vertexBuffer->count = mesh->verticies.size();
    vertexBuffer->sizet = mesh->verticies.size() * sizeof(Vertex);
    std::cout << vertexBuffer->sizet << " is the total Size" << std::endl;
    indexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
    indexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::INDEX;
    indexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    indexBuffer->data = mesh->indicies.data();
    indexBuffer->count = mesh->indicies.size();
    indexBuffer->sizet = mesh->indicies.size() * sizeof(uint32_t);
    std::cout << indexBuffer->sizet << " is the total size " << std::endl;
    mesh->indexBuffer = indexBuffer;
    mesh->vertexBuffer = vertexBuffer;
}



