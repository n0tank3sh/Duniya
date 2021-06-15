#include "GLRenderer.h"

GLRenderer::GLRenderer(void* GlProcAddressLoader)
{
    if(!gladLoadGLLoader((GLADloadproc) GlProcAddressLoader))
    {

    }
}
