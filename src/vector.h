//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <cassert>
#include <cmath>
#include "common.h"

#include <glm/glm.hpp>

namespace rays {

    inline int maxDimension(const glm::vec3 &v) {
        return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
    }

	inline float MaxComponent(const glm::vec3 &v) {
		return std::max(v.x, std::max(v.y, v.z));
	}

    inline glm::vec3 permute(const glm::vec3 &v, int x, int y, int z) {
        return glm::vec3(v[x], v[y], v[z]);
    }

	constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon() * 0.5f;

	inline float gamma(int n) {
		return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
	}


}
#endif //RAYTRACER_VECTOR_H
