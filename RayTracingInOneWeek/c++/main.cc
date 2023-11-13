#include <iostream>
#include <time.h>
using std::cout;

int main(){
    const int img_wid = 256;
    const int img_height = 256;

    cout<< "P3\n" << img_wid << ' ' << img_height <<"\n255\n";
    clock_t start = clock();

    for(int j = 0; j < img_wid ; j++){
        for(int i = 0; i < img_wid; ++i){
            auto r = double(i) / (img_wid-1);
            auto g = double(j) / (img_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    clock_t end = clock();
    double timer_seconds = ((double)(end - start)) / CLOCKS_PER_SEC;
    std::cerr << "took " << timer_seconds << " seconds.\n";
}