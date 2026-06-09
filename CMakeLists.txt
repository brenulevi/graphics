cmake_minimum_required(VERSION 3.10)

project(MyGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# GLOBAL: NÃO transformar warnings em errors (externals quebram fácil)
# --------------------------------------------------
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
add_compile_options(-Wno-error)

# --------------------------------------------------
# GLFW options (DEVEM vir antes do add_subdirectory)
# --------------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# --------------------------------------------------
# Disable strict warnings BEFORE external libs
# --------------------------------------------------
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -Wno-error=maybe-uninitialized")

# IMPORTANT: prevent Assimp enabling Werror internally
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/assimp)

# --------------------------------------------------
# External libs
# --------------------------------------------------
add_subdirectory(external/glfw)
add_subdirectory(external/glad/cmake)
add_subdirectory(external/glm)

glad_add_library(glad STATIC API gl:core=3.3)

# --------------------------------------------------
# OpenGL (cross compile safe)
# --------------------------------------------------
find_package(OpenGL REQUIRED)

# fallback mais seguro para MinGW cross
if (CMAKE_CROSSCOMPILING AND WIN32)
    set(OPENGL_LIB opengl32)
else()
    set(OPENGL_LIB OpenGL::GL)
endif()

# --------------------------------------------------
# Executável
# --------------------------------------------------
add_executable(MyGame
    src/main.cpp

    src/window.h
    src/window.cpp
    src/application.h
    src/application.cpp
    src/input.h
    src/input.cpp

    src/resources/assetmanager.h
    src/resources/assetmanager.cpp
    src/resources/meshloader.h
    src/resources/meshloader.cpp
    src/resources/textureloader.h
    src/resources/textureloader.cpp
    src/resources/shaderloader.h
    src/resources/shaderloader.cpp
    src/resources/modelloader.h
    src/resources/modelloader.cpp

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
    src/rendering/model.h

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
    src/components/light.h
    src/components/directionallight.h
    src/components/pointlight.h
    src/components/playercontroller.h
    src/components/playercontroller.cpp
    src/components/spotlight.h
)

# --------------------------------------------------
# Includes
# --------------------------------------------------
target_include_directories(MyGame PRIVATE
    src
    external/stb_image
    external/glm
    external/glad/include
    external/glfw/include
    external/assimp/include
)

# --------------------------------------------------
# Definitions
# --------------------------------------------------
target_compile_definitions(MyGame PRIVATE
    GLFW_INCLUDE_NONE
    GLM_ENABLE_EXPERIMENTAL
)

# --------------------------------------------------
# Linking (cross-safe)
# --------------------------------------------------
target_link_libraries(MyGame PRIVATE
    glfw
    glad
    glm
    assimp
    ${OPENGL_LIB}
)

# --------------------------------------------------
# Assimp safety: remove Werror localmente (extra segurança)
# --------------------------------------------------
set_target_properties(assimp PROPERTIES
    COMPILE_WARNING_AS_ERROR OFF
)

# --------------------------------------------------
# Assets copy
# --------------------------------------------------
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:MyGame>/assets
)