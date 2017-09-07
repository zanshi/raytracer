//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "common.h"
#include "vertex.h"
#include "pixel.h"


namespace rays {

    class Camera {
    public:
        //Camera(std::initializer_list<Vertex> eyeList, unsigned int x, unsigned int y);

        Camera(std::initializer_list<Vertex> eyes, unsigned int x, unsigned int y, unsigned int currEyeIdx = 0);

        void render();

        void createImage() const;

        std::vector<Vertex> eyes;
        std::vector<std::vector<Pixel>> plane;

        unsigned int currEyeIdx{0};

    };

}


#endif //RAYTRACER_CAMERA_H
