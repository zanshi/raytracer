//
// Created by Niclas Olmenius on 2017-09-07.
//

#include <scene.h>
#include "common.h"
#include "camera.h"
#include "triangle.h"
#include "vertex.h"
#include "sphere.h"

using namespace rays;

std::vector<std::shared_ptr<Shape>> createWorld() {

    // Define vertices
    // Define indices
    // Create triangle mesh object from the vertices and indices
    // return triangle mesh?
    // or, return vector of triangles?

    // mesh:
    // consolidated
    // well structured

    std::vector<Triangle> scene;
    // Floor
    // Unique vertex list
    auto vertices = std::make_unique<std::vector<Vertex>>();

    Vertex v0(-3.0, 0.0, -5.0, 1.0);
    Vertex v1(0.0, -6.0, -5.0, 1.0);
    Vertex v2(10.0, -6.0, -5.0, 1.0);
    Vertex v3(13.0, 0.0, -5.0, 1.0);
    Vertex v4(10.0, 6.0, -5.0, 1.0);
    Vertex v5(0.0, 6.0, -5.0, 1.0);

    // Ceiling
    Vertex v6(-3.0, 0.0, 5.0, 1.0);
    Vertex v7(0.0, -6.0, 5.0, 1.0);
    Vertex v8(10.0, -6.0, 5.0, 1.0);
    Vertex v9(13.0, 0.0, 5.0, 1.0);
    Vertex v10(10.0, 6.0, 5.0, 1.0);
    Vertex v11(0.0, 6.0, 5.0, 1.0);

    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);

    ColorDbl lightred{1.0, 0.5, 0.5};
    ColorDbl lightgreen{0.5, 1.0, 0.5};
    ColorDbl lightblue{0.5, 0.5, 1.0};
    ColorDbl white{1, 1, 1};
    ColorDbl yellow{1, 1, 0};
    ColorDbl magenta{1, 0, 1};
    ColorDbl cyan{0, 1, 1};

    // Transfer ownership of vertices to the triangle mesh
    auto mesh = std::make_shared<TriangleMesh>(std::move(vertices));

    // Connect everything
    // Floor
//    Triangle t0 ({v0, v1, v5}, {0, 0, 1}, white);
    Triangle t0(mesh, {0, 1, 5}, {0, 0, 1}, white);
    Triangle t1(mesh, {1, 2, 5}, {0, 0, 1}, white);
    Triangle t2(mesh, {2, 4, 5}, {0, 0, 1}, white);
    Triangle t3(mesh, {2, 3, 4}, {0, 0, 1}, white);

    // Ceiling
    Triangle t4{mesh, {7, 6, 11}, {0, 0, -1}, white};
    Triangle t5{mesh, {7, 11, 8}, {0, 0, -1}, white};
    Triangle t6{mesh, {8, 11, 10}, {0, 0, -1}, white};
    Triangle t7{mesh, {8, 10, 9}, {0, 0, -1}, white};

    // -y wall (fix normals)
    Triangle t8{mesh, {0, 6, 1}, {0, 0, -1}, yellow};
    Triangle t9{mesh, {1, 6, 7}, {0, 0, -1}, yellow};
    Triangle t10{mesh, {1, 7, 2}, {0, -1, 0}, magenta};
    Triangle t11{mesh, {2, 7, 8}, {0, -1, 0}, magenta};
    Triangle t12{mesh, {2, 8, 3}, {0, 0, -1}, cyan};
    Triangle t13{mesh, {3, 8, 9}, {0, 0, -1}, cyan};

    // +y wall (fix normals)
    Triangle t14{mesh, {0, 5, 11}, {0, 0, -1}, lightblue};
    Triangle t15{mesh, {0, 11, 6}, {0, 0, -1}, lightblue};
    Triangle t16{mesh, {5, 4, 10}, {0, 0, 1}, lightred};
    Triangle t17{mesh, {5, 10, 11}, {0, 0, 1}, lightred};
    Triangle t18{mesh, {4, 3, 9}, {0, 0, -1}, lightgreen};
    Triangle t19{mesh, {4, 9, 10}, {0, 0, -1}, lightgreen};

    std::vector<std::shared_ptr<Shape>> tris;

    tris.push_back(std::make_shared<Triangle>(t0));
    tris.push_back(std::make_shared<Triangle>(t1));
    tris.push_back(std::make_shared<Triangle>(t2));
    tris.push_back(std::make_shared<Triangle>(t3));
    tris.push_back(std::make_shared<Triangle>(t4));
    tris.push_back(std::make_shared<Triangle>(t5));
    tris.push_back(std::make_shared<Triangle>(t6));
    tris.push_back(std::make_shared<Triangle>(t7));
    tris.push_back(std::make_shared<Triangle>(t8));
    tris.push_back(std::make_shared<Triangle>(t9));
    tris.push_back(std::make_shared<Triangle>(t10));
    tris.push_back(std::make_shared<Triangle>(t11));
    tris.push_back(std::make_shared<Triangle>(t12));
    tris.push_back(std::make_shared<Triangle>(t13));
    tris.push_back(std::make_shared<Triangle>(t14));
    tris.push_back(std::make_shared<Triangle>(t15));
    tris.push_back(std::make_shared<Triangle>(t16));
    tris.push_back(std::make_shared<Triangle>(t17));
    tris.push_back(std::make_shared<Triangle>(t18));
    tris.push_back(std::make_shared<Triangle>(t19));

    return tris;
}

int main() {

    // Create scene
    std::vector<std::shared_ptr<Shape>> world = createWorld();
    auto sphere = std::make_shared<Sphere>(Sphere({1, 0, 0}, {5, 0, 2, 1}, 1.5f));
    world.insert(world.begin(), sphere);
//    world.push_back(sphere);

    auto scene = Scene(std::move(world));

    std::vector<Vertex> eyes(2);
    eyes[0] = Vertex(-2.0, 0, 0, 1);
    eyes[1] = Vertex(-0.5, 0, 0, 1);

    auto camera = Camera(eyes, 1000, 1000, 1);

    // Render the scene
    camera.render(scene);

    // Save an image
    camera.createImage();

    return 0;
}