# Ray Tracing Image Generation with CUDA

## Overview

This project generates high-quality ray-traced images leveraging the power of NVIDIA CUDA for accelerated computing. It illustrates the application of various materials to spheres in a randomly generated scene, demonstrating the capability of CUDA for intensive parallel computations in graphics rendering.

## Prerequisites

- NVIDIA CUDA Toolkit (Ensure the version installed is compatible with your GPU)
- An NVIDIA GPU that supports the architecture specified in the Makefile (default is set for Tesla T4 with Turing architecture)
- GNU Make
- GNU G++ or another host compiler compatible with CUDA

## Building the Project

The project includes a `Makefile` for easy compilation. To build the project, follow these steps:

1. **Open a Terminal in the Project Directory**

2. **Set the CUDA Path (optional)**

   If CUDA is not installed in `/usr/local/cuda`, set the `CUDA_PATH` variable to the correct path. This can be done within the `Makefile` or as an environment variable:

   ```bash
   export CUDA_PATH=/path/to/your/cuda
   ```
3. **Compile the Project**

   ```bash
   make all
   ```
   This command compiles the cudart executable. If your GPU architecture is different from Tesla T4 (compute capability 7.5), modify the GENCODE_FLAGS in the Makefile to match your GPU's architecture.

## Running the Program
After building the project, generate an image with:
```bash
./cudart > image.ppm
```
This will create a PPM file image.ppm that contains the ray-traced image.

## Profiling with nvprof
The Makefile also includes a target for profiling the executable using nvprof to gather performance metrics:
```bash
make profile_metrics
```

