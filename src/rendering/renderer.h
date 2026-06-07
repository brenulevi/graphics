#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void clear() const;

    static inline Renderer* getInstance() { return s_instance; }

private:
    static Renderer* s_instance;
};