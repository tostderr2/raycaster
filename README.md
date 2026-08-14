# C++ Graphics & Engine Development Projects

A monorepo tracing my progression through low-level 2D software rendering, pseudo-3D projections, CPU rasterization, hardware graphics APIs, and ray tracing.

Every subfolder contains a standalone, fully executable project with Cmakelist.txt built with C++17/20

---

## Project Details

### 01. Pong Software Renderer
> `01_software_2d/01_pong_renderer`

![Pong Preview](docs/previews/pong.gif)

* **Key Features & Math:** 1D Raw Framebuffer Manipulation, Bresenham's Line Algorithm, AABB Collision & Delta-Time Loop.
* **Core Concepts:** Pixel buffer indexing, software line rendering, basic game loop state management.

---

### 02. Asteroids Vector Game
> `01_software_2d/02_asteroids_vector`

![Asteroids Preview](docs/previews/asteroids.gif)

* **Key Features & Math:** 2D Matrix Transformations ($2 \times 2$ Rotation & Scaling), Vector Momentum & Angular Velocity Integration, Point-in-Polygon Collision.
* **Core Concepts:** Dynamic vector graphics, affine 2D transformations, physics state updates.

---

### 03. Tilemap Platformer
> `01_software_2d/03_tilemap_platformer`

![Platformer Preview](docs/previews/platformer.gif)

* **Key Features & Math:** Swept AABB Tilemap Collision Resolution, Frame-based Sprite Sheet Animation, Camera Tracking & View Matrices.
* **Core Concepts:** Tile grid parsing, sub-pixel physics integration, camera translation logic.

---

### 04. DDA Wolfenstein Raycaster
> `02_pseudo_3d/01_wolfenstein_raycaster`

![Raycaster Preview](docs/previews/raycaster.gif)

* **Key Features & Math:** Digital Differential Analysis (DDA) Grid Traversal, Distance-to-Height Perspective Projection, Affine Texture Sampling & Shading.
* **Core Concepts:** Pseudo-3D raycasting, wall distance correction (fish-eye removal), vertical slice rendering.

---

### 05. Retro Mode 7 Racer
> `02_pseudo_3d/02_mode7_racer`

![Mode 7 Preview](docs/previews/mode7.gif)

* **Key Features & Math:** Perspective Ground Plane Projection, Real-time Affine Map Transformation, SNES-style Camera Math.
* **Core Concepts:** Horizon projection scaling, scanning-line texture sampling, track rotation.

---

### 06. Wireframe 3D OBJ Viewer
> `03_software_3d/01_wireframe_viewer`

![Wireframe Preview](docs/previews/wireframe.gif)

* **Key Features & Math:** Custom $4 \times 4$ MVP Matrix Pipeline, Frustum Clipping & Perspective Division, Custom `.obj` Parser.
* **Core Concepts:** 3D homogeneous coordinates, camera view transforms, wireframe line clipping.

---

### 07. CPU Software Rasterizer
> `03_software_3d/02_software_rasterizer`

![Rasterizer Preview](docs/previews/rasterizer.gif)

* **Key Features & Math:** Barycentric Coordinate Triangle Rasterization, Z-Buffer Depth Testing, Perspective-Correct Texture Interpolation.
* **Core Concepts:** Software graphics pipeline, fragment interpolation, flat/Gouraud shading.

---

### 08. Interactive CPU Ray Tracer
> `05_raytracing/01_cpu_raytracer`

![Ray Tracer Preview](docs/previews/raytracer.gif)

* **Key Features & Math:** Ray-Sphere & Ray-Triangle Intersections, Material Models (Lambertian, Metal, Dielectric), BVH (Bounding Volume Hierarchy) Acceleration.
* **Core Concepts:** Global illumination, path tracing, spatial partitioning trees.

##  Tech Stack & Standards

* **Language:** C++17 / C++20
* **Windowing & Pixel Display:** SDL3 / SDL2
* **Build System:** CMake 3.16+
* **Code Style:** LLVM-based strict formatting (`.clang-format`)
* **Tooling:** Neovim + `clangd`, GCC / Clang / MSVC

---

##  Building & Running

This monorepo is configured with a unified root CMake setup. You can configure and build all projects simultaneously in two commands.

### Prerequisites

Ensure you have a modern C++ compiler (`g++`, `clang++`, or MSVC) and `cmake` installed.

#### Linux (Debian/Ubuntu/Mint)
```bash
sudo apt install build-essential cmake libsdl3-dev
```

#### MacOs (Homebrew)
```bash
brew install cmake sdl3
```

#### Build Instructions

1. Clone the repository:

```Bash
git clone https://github.com/your-username/graphics-game-dev-roadmap.git
cd graphics-game-dev-roadmap
```

2. Configure with CMake:

```Bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

3. Compile ALL targets:

```bash
# to build everything
cmake --build build --parallel
```

```bash
# to just build one of the project
cmake --build build --target wolfenstein_raycaster

```

 although this way of doing is a bit stupid.

 so just go inside the directory and build it there, 
```bash
cmake --build build --parallel
```

4. Run any executable target:
```bash
# Example: Launching the Wolfenstein Raycaster
./build/bin/wolfenstein_raycaster
```
