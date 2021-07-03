#include "Renderer.h"

void Renderer::SetLayout(VertexSpecification& specification)
{
	this->specification = specification;
}
 
void Texture::Bind() const noexcept
{
        binder->Bind();
}

void Texture::UnBind() const noexcept
{
        binder->UnBind();
}
void GBuffer::Bind() const noexcept
{
        if(gBinder.get() == nullptr) std::cout << "Gbinder is null " << std::endl;
        gBinder->Bind();
}    
void GBuffer::UnBind() const noexcept
{
        gBinder->UnBind();
}
