#include "renderer.h"

Renderer* Renderer::s_instance = nullptr;

Renderer::Renderer()
{
    if(s_instance)
    {
        throw std::runtime_error("Renderer instance already exists!");
    }

    s_instance = this;

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD!");
    }
}

Renderer::~Renderer()
{
    s_instance = nullptr;
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
