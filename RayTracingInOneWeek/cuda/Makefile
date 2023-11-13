all: render_image

render_image: main.cu
    nvcc -o main main.cu

profile: render_image
    nvprof --print-gpu-summary ./main

clean:
    rm -f main
