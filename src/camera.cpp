//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"

namespace rays {
    Camera::Camera(std::initializer_list<Vertex> eyes, unsigned int x, unsigned int y,
                   unsigned int currEyeIdx)
            : eyes(eyes), plane(x, std::vector<Pixel>(y)), currEyeIdx(currEyeIdx) {}

    void Camera::render() {

    }

    void Camera::createImage() const {

    }
}
