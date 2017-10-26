//
// Created by Niclas Olmenius on 2017-09-07.
//
#include "common.h"

#include "scenebuilder.h"
#include "scene.h"
#include "camera.h"

using namespace rays;


Camera setupCamera() {
    // Create camera "eyes"
    std::array<glm::vec3, 2> cameraPositions = {
            glm::vec3{-2.0f, 0.f, 0.f},
            glm::vec3{-1.0f, 0.f, 0.f}
    };

    return Camera(cameraPositions);
}

int main() {

    SceneBuilder sceneBuilder;
    const Scene scene = sceneBuilder.setupScene();
    Camera camera = setupCamera();

    // Render the scene
    auto t0 = std::chrono::system_clock::now();
    camera.render(scene);
    auto end = std::chrono::system_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(end - t0);
    std::cout << "Total rendering time: " << totalTime.count() << " seconds. " << std::endl;

    // Save an image to out.png
    std::stringstream ss;
    ss << "out-";
    ss << Options::width << "x" << Options::width << "-";
    ss << Options::nrSamples;

    camera.createImage(ss.str());
    camera.createRawImage(ss.str());

    return 0;
}