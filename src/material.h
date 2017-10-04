//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "common.h"
#include "colordbl.h"
#include "bsdf.h"

namespace rays {

    // Need:
    // BRDF
    struct Material {
        explicit Material(const std::shared_ptr<BSDF> &brdf) : brdf(brdf) {}
        const std::shared_ptr<BSDF> brdf;

    };

}

#endif //RAYTRACER_MATERIAL_H
