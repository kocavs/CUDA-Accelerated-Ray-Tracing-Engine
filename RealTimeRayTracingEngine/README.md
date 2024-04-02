# Real-Time Ray Tracing Engine

## Overview

This project is a real-time ray tracing engine developed in C++. It utilizes GLFW for window management and Dear ImGui for an interactive GUI. The engine renders a dynamic scene consisting of spheres with various materials, and allows for real-time scene adjustments via the GUI.

## Prerequisites

- CMake (version 3.10 or higher recommended)
- A C++ compiler compatible with C++11 or higher (e.g., GCC, Clang)
- GLFW
- OpenGL
- Dear ImGui

## Building the Project

This project uses CMake for easy compilation and building. Follow these steps to compile the project:

1. **Clone the Repository**

2. **Navigate to the Project Directory**

3. **Create a Build Directory**

    ```bash
    mkdir build
    cd build
    ```

4. **Generate Build Configuration**

    Run CMake to configure the project. Adjust paths to GLFW and Dear ImGui if they are installed in non-standard locations.

    ```bash
    cmake ..
    ```

5. **Compile the Project**

    Compile the project with:

    ```bash
    make
    ```

    This will generate an executable named `RayTracing`.

## Running the Engine

After compiling, you can start the engine by running:

```bash
./RayTracing
```

## Controls
- Use the ImGui GUI to adjust scene and material properties.
- Navigate the camera with keyboard inputs (W, A, S, D).
