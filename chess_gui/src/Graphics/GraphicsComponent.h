
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
    std::unique_ptr<GBuffer> vertexBuffer;
    std::unique_ptr<GBuffer> indexbuffer;
    std::unique_ptr<Texture> texture;
};


