#include <iostream>
#include <memory>

#include "window.h"
#include "rendering/renderer.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    std::unique_ptr<Window> window = std::make_unique<Window>(800, 600, "My Game");
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

    while (!window->shouldClose())
    {
        window->pollEvents();

        renderer->clear();

        window->swapBuffers();
    }

    return 0;
}