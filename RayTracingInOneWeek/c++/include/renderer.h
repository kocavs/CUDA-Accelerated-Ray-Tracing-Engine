#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "hittableList.h"

class Renderer {
public:
    Renderer(int width, int height, int samplesPerPixel, int maxDepth);
    void setScene(const hittableList& world, const Camera& camera);
    void renderScene();
    const std::vector<unsigned char>& getPixels() const;
    void updateCamera(const Camera &cam);

private:
    int imgWidth, imgHeight, spp, maxDepth;
    Camera camera;
    hittableList world;
    std::vector<unsigned char> pixels;

    color ray_color(const ray& r, hittableList world, int depth);
    // Other private methods and members...
};

#endif
