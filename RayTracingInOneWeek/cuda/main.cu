#include <iostream>
#include <cuda_runtime.h>
#include <time.h>

__global__ void render(float *img, int max_x, int max_y) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    if((i >= max_x) || (j >= max_y)) return;

    float r = float(i) / (max_x - 1);
    float g = float(j) / (max_y - 1);
    float b = 0.25;

    int pixel_index = j * max_x * 3 + i * 3;
    img[pixel_index + 0] = r;
    img[pixel_index + 1] = g;
    img[pixel_index + 2] = b;
}

int main() {
    const int img_wid = 256;
    const int img_height = 256;
    const int img_size = img_wid * img_height * 3;
    float *img_buffer;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaMallocManaged(&img_buffer, img_size * sizeof(float));

    // Start record
    cudaEventRecord(start);
    dim3 blocks(img_wid / 16, img_height / 16);
    dim3 threads(16, 16);
    
    render<<<blocks, threads>>>(img_buffer, img_wid, img_height);

    cudaDeviceSynchronize();
    
    // Stop record
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    // Calculate elapsed time
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    std::cerr << "Image generation took " << milliseconds << " milliseconds.\n";


    std::cout << "P3\n" << img_wid << ' ' << img_height << "\n255\n";
    for(int j = 0; j < img_height; ++j) {
        for(int i = 0; i < img_wid; ++i) {
            int pixel_index = j * img_wid * 3 + i * 3;
            float r = img_buffer[pixel_index + 0];
            float g = img_buffer[pixel_index + 1];
            float b = img_buffer[pixel_index + 2];

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    cudaFree(img_buffer);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}
