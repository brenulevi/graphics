cmake_minimum_required(VERSION 3.10)

project(MyGame)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("external/glfw")
add_subdirectory("external/glad/cmake")
add_subdirectory("external/glm")

glad_add_library(glad STATIC API gl:core=3.3)

find_package(OpenGL REQUIRED)

add_executable(MyGame
    src/main.cpp
    src/window.h
    src/window.cpp

    src/rendering/shader.h
    src/rendering/shader.cpp
    src/rendering/vertexbuffer.h
    src/rendering/vertexbuffer.cpp
    src/rendering/indexbuffer.h
    src/rendering/indexbuffer.cpp
    src/rendering/bufferlayout.h
    src/rendering/bufferlayout.cpp
    src/rendering/vertexarray.h
    src/rendering/vertexarray.cpp
    src/rendering/renderer.h
    src/rendering/renderer.cpp

    src/scenering/scene.h
    src/scenering/scene.cpp
    src/scenering/gameobject.h
    src/scenering/gameobject.cpp
    
    src/components/component.h
)

target_include_directories(MyGame PRIVATE
    src
)

target_compile_definitions(MyGame PRIVATE
    GLFW_INCLUDE_NONE
)

target_link_libraries(MyGame glfw OpenGL::GL glad glm)