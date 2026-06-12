# Graphics

A small C++20 OpenGL 3.3 engine with a component-based scene system. The demo scene renders textured meshes, loads OBJ models via Assimp, and supports directional shadow mapping, point/spot lights, and a skybox.

## Features

- **Scene graph** — `GameObject` hierarchy with `Transform`, `MeshRenderer`, `Camera`, and light components
- **Rendering** — Blinn-Phong shading with diffuse/specular maps, shadow maps, and cubemap skybox
- **Asset pipeline** — Shaders, textures, cubemaps, and models loaded through `AssetManager`
- **Input** — First-person `PlayerController` with mouse look

## Requirements

- CMake 3.10+
- C++20 compiler (GCC 11+, Clang, or MSVC)
- OpenGL 3.3+ capable GPU and drivers
- Python 3 (used by GLAD to generate OpenGL loaders on first build)

On Linux, GLFW may also require development packages for X11 and/or Wayland (e.g. on Ubuntu: `libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`).

## Getting Started

Clone the repository with submodules:

```bash
git clone --recursive git@github.com:brenulevi/graphics.git
cd graphics
```

If you already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

Build and run:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/MyGame
```

Assets are copied into `build/assets/` during the build. Run the executable from the project root (as above) or from inside `build/` — both work because paths are relative to the working directory.

The first build downloads the OpenGL specification to generate GLAD bindings, so an internet connection is required once.

## Controls

| Input | Action |
|-------|--------|
| `W` / `A` / `S` / `D` | Move forward / left / back / right |
| `Space` / `Left Ctrl` | Move up / down |
| Mouse | Look around |
| `Escape` | Toggle cursor capture |
| Left click | Re-capture cursor when released |

## Project Structure

```
src/
├── application.*       # Main loop, scene setup
├── window.*            # GLFW window
├── input.*             # Keyboard and mouse input
├── components/         # Transform, Camera, lights, MeshRenderer, PlayerController
├── rendering/          # Renderer, shaders, meshes, materials, shadow maps
├── scenering/          # Scene, GameObject, RenderSettings
├── resources/          # AssetManager and loaders
└── systems/            # RenderSystem (render passes)

assets/
├── shaders/            # GLSL vertex/fragment shaders
├── textures/           # Diffuse, specular, and skybox cubemap faces
└── models/             # OBJ models (e.g. backpack)

external/               # Git submodules: GLFW, GLAD, GLM, Assimp + stb_image
```

## Render Pipeline

`RenderSystem` orchestrates each frame in distinct passes:

1. **Context** — Validate camera, gather mesh renderers, build `SceneData`
2. **Shadow pass** — Render depth from the directional light into a shadow map
3. **Light collection** — Gather point and spot light data for the shader
4. **Main pass** — Draw skybox and lit meshes

## Dependencies

| Library | Purpose |
|---------|---------|
| [GLFW](https://www.glfw.org/) | Window and input |
| [GLAD](https://github.com/Dav1dde/glad) | OpenGL loader |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) |
| [Assimp](https://github.com/assimp/assimp) | Model loading |
| [stb_image](https://github.com/nothings/stb) | Texture loading |

Each dependency lives under `external/` as a git submodule or vendored source.
