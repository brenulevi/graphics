cmake_minimum_required(VERSION 3.10)

project(MyGame)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

add_subdirectory("external/glfw")
add_subdirectory("external/glad/cmake")
add_subdirectory("external/glm")

glad_add_library(glad STATIC API gl:core=3.3)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OPENGL_gl_LIBRARY opengl32)
endif()

find_package(OpenGL REQUIRED)

add_executable(MyGame
    src/main.cpp

    src/window.h
    src/window.cpp
    src/application.h
    src/application.cpp
    src/input.h
    src/input.cpp

    src/rendering/shader.h
    src/rendering/shader.cpp
    src/rendering/vertexbuffer.h
    src/rendering/vertexbuffer.cpp
    src/rendering/indexbuffer.h
    src/rendering/indexbuffer.cpp
    src/rendering/bufferlayout.h
    src/rendering/vertexarray.h
    src/rendering/vertexarray.cpp
    src/rendering/renderer.h
    src/rendering/renderer.cpp
    src/rendering/vertex.h
    src/rendering/mesh.h
    src/rendering/mesh.cpp
    src/rendering/material.h
    src/rendering/texture.h
    src/rendering/texture.cpp

    src/scenering/scene.h
    src/scenering/scene.cpp
    src/scenering/gameobject.h
    src/scenering/gameobject.cpp
    
    src/systems/rendersystem.h
    src/systems/rendersystem.cpp

    src/components/component.h
    src/components/transform.h
    src/components/transform.cpp
    src/components/meshrenderer.h
    src/components/camera.h
    src/components/rotationcomponent.h
    src/components/rotationcomponent.cpp
    src/components/translatecomponent.h
    src/components/translatecomponent.cpp
    src/components/light.h
    src/components/directionallight.h
    src/components/pointlight.h
    src/components/playercontroller.h
    src/components/playercontroller.cpp
    src/components/spotlight.h
)

target_include_directories(MyGame PRIVATE
    src
    external/stb_image
)

target_compile_definitions(MyGame PRIVATE
    GLFW_INCLUDE_NONE
)

target_link_libraries(MyGame
    glfw
    OpenGL::GL
    glad
    glm
)

add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:MyGame>/assets
)