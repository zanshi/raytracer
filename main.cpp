//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "common.h"
#include "camera.h"
#include "triangle.h"

using namespace rays;
std::vector<Triangle> setupScene() {
    std::vector<Triangle> scene;
    // Floor
    Vertex v0 {-3.0, 0.0, -5.0};
    Vertex v1 {0.0, -6.0, -5.0};
    Vertex v2 {10.0, -6.0, -5.0};
    Vertex v3 {13.0, 0.0, -5.0};
    Vertex v4 {10.0, 6.0, -5.0};
    Vertex v5 {0.0, 6.0, -5.0};

    // Ceiling
    Vertex v6 {-3.0, 0.0, 5.0};
    Vertex v7 {0.0, -6.0, 5.0};
    Vertex v8 {10.0, -6.0, 5.0};
    Vertex v9 {13.0, 0.0, 5.0};
    Vertex v10 {10.0, 6.0, 5.0};
    Vertex v11 {0.0, 6.0, 5.0};

    std::vector<Vertex> vertices;
    vertices.emplace_back(v0, v1, v2, v3, v4,
                          v5, v6, v7, v8, v9,
                          v10, v11);

    // Connect everything
    // Floor
    Triangle t0 {{v0, v1, v5}, {0, 0, 1}, {1, 1, 1}};
    Triangle t1 {{v1, v2, v5}, {0, 0, 1}, {1, 1, 1}};
    Triangle t2 {{v2, v4, v5}, {0, 0, 1}, {1, 1, 1}};
    Triangle t3 {{v2, v3, v4}, {0, 0, 1}, {1, 1, 1}};

    // Ceiling
    Triangle t4 {{v7, v6, v11}, {0, 0, -1}, {1, 1, 1}};
    Triangle t5 {{v7, v11, v8}, {0, 0, -1}, {1, 1, 1}};
    Triangle t6 {{v8, v11, v10}, {0, 0, -1}, {1, 1, 1}};
    Triangle t7 {{v8, v10, v9}, {0, 0, -1}, {1, 1, 1}};

    // -y wall (fix normals)
    Triangle t8 {{v0, v6, v1}, {0, 0, -1}, {1, 1, 1}};
    Triangle t9 {{v1, v6, v7}, {0, 0, -1}, {1, 1, 1}};
    Triangle t10 {{v1, v7, v2}, {0, -1, 0}, {1, 1, 1}};
    Triangle t11 {{v2, v7, v8}, {0, -1, 0}, {1, 1, 1}};
    Triangle t12 {{v2, v8, v3}, {0, 0, -1}, {1, 1, 1}};
    Triangle t13 {{v3, v8, v9}, {0, 0, -1}, {1, 1, 1}};

    // +y wall (fix normals)
    Triangle t14 {{v0, v5, v11}, {0, 0, -1}, {1, 1, 1}};
    Triangle t15 {{v0, v11, v6}, {0, 0, -1}, {1, 1, 1}};
    Triangle t16 {{v5, v4, v10}, {0, 0, 1}, {1, 1, 1}};
    Triangle t17 {{v5, v10, v11}, {0, 0, 1}, {1, 1, 1}};
    Triangle t18 {{v4, v3, v9}, {0, 0, -1}, {1, 1, 1}};
    Triangle t19 {{v4, v9, v10}, {0, 0, -1}, {1, 1, 1}};




}

int main() {

    std::vector<Triangle> scene;
    // Add floor

    auto camera = Camera({{0.0, 0.0, 0.0},
                              {0.0, 0.1, 0.0}}, 1000, 1000);
        std::cout << camera.plane.size() << std::endl;
    return 0;
}