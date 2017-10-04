//
// Created by Niclas Olmenius on 2017-09-07.
//

#include <scene.h>
#include <point_light.h>
#include "common.h"
#include "camera.h"
#include "triangle.h"
#include "vertex.h"
#include "sphere.h"
#include "sceneobject.h"

using namespace rays;

std::vector<std::shared_ptr<SceneObject>> createWorld() {

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
    auto vertices = std::make_unique<std::vector<Vertex3f>>();

    Vertex3f v0(-3.0, 0.0, -5.0);
    Vertex3f v1(0.0, -6.0, -5.0);
    Vertex3f v2(10.0, -6.0, -5.0);
    Vertex3f v3(13.0, 0.0, -5.0);
    Vertex3f v4(10.0, 6.0, -5.0);
    Vertex3f v5(0.0, 6.0, -5.0);

    // Ceiling
    Vertex3f v6(-3.0, 0.0, 5.0);
    Vertex3f v7(0.0, -6.0, 5.0);
    Vertex3f v8(10.0, -6.0, 5.0);
    Vertex3f v9(13.0, 0.0, 5.0);
    Vertex3f v10(10.0, 6.0, 5.0);
    Vertex3f v11(0.0, 6.0, 5.0);

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

    Material diffuseRed {};

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
    // Floor, white
    Triangle t0(mesh, {0, 1, 5}, {0, 0, 1});
    Triangle t1(mesh, {1, 2, 5}, {0, 0, 1});
    Triangle t2(mesh, {2, 4, 5}, {0, 0, 1});
    Triangle t3(mesh, {2, 3, 4}, {0, 0, 1});

    // Ceiling, white
    Triangle t4{mesh, {7, 6, 11}, {0, 0, -1}};
    Triangle t5{mesh, {7, 11, 8}, {0, 0, -1}};
    Triangle t6{mesh, {8, 11, 10}, {0, 0, -1}};
    Triangle t7{mesh, {8, 10, 9}, {0, 0, -1}};

    // -y wall (fix normals), different colours
    Triangle t8{mesh, {0, 6, 1}, {0, 0, -1}};
    Triangle t9{mesh, {1, 6, 7}, {0, 0, -1}};
    Triangle t10{mesh, {1, 7, 2}, {0, -1, 0}};
    Triangle t11{mesh, {2, 7, 8}, {0, -1, 0}};
    Triangle t12{mesh, {2, 8, 3}, {0, 0, -1}};
    Triangle t13{mesh, {3, 8, 9}, {0, 0, -1}};

    // +y wall (fix normals), different colours
    Triangle t14{mesh, {0, 5, 11}, {0, 0, -1}};
    Triangle t15{mesh, {0, 11, 6}, {0, 0, -1}};
    Triangle t16{mesh, {5, 4, 10}, {0, 0, 1}};
    Triangle t17{mesh, {5, 10, 11}, {0, 0, 1}};
    Triangle t18{mesh, {4, 3, 9}, {0, 0, -1}};
    Triangle t19{mesh, {4, 9, 10}, {0, 0, -1}};

//    std::vector<std::shared_ptr<Shape>> tris;
    Lambertian wall_white(white, )

    Material wall_white()

    std::vector<std::shared_ptr<SceneObject>> objects;

    objects.emplace_back(std::make_shared(SceneObject(t0,)))

    tris.emplace_back(std::make_shared<Triangle>(t0));

    tris.emplace_back(std::make_shared<Triangle>(t1));
    tris.emplace_back(std::make_shared<Triangle>(t2));
    tris.emplace_back(std::make_shared<Triangle>(t3));
    tris.emplace_back(std::make_shared<Triangle>(t4));
    tris.emplace_back(std::make_shared<Triangle>(t5));
    tris.emplace_back(std::make_shared<Triangle>(t6));
    tris.emplace_back(std::make_shared<Triangle>(t7));
    tris.emplace_back(std::make_shared<Triangle>(t8));
    tris.emplace_back(std::make_shared<Triangle>(t9));
    tris.emplace_back(std::make_shared<Triangle>(t10));
    tris.emplace_back(std::make_shared<Triangle>(t11));
    tris.emplace_back(std::make_shared<Triangle>(t12));
    tris.emplace_back(std::make_shared<Triangle>(t13));
    tris.emplace_back(std::make_shared<Triangle>(t14));
    tris.emplace_back(std::make_shared<Triangle>(t15));
    tris.emplace_back(std::make_shared<Triangle>(t16));
    tris.emplace_back(std::make_shared<Triangle>(t17));
    tris.emplace_back(std::make_shared<Triangle>(t18));
    tris.emplace_back(std::make_shared<Triangle>(t19));



    return tris;
}


Scene setupScene() {
   // Create scene
    std::vector<std::shared_ptr<SceneObject>> world = createWorld();
    // Add a sphere
    auto sphere = std::make_shared<Sphere>(Sphere({5, 0, 2}, 1.5f));
    world.insert(world.begin(), sphere);
//    world.push_back(sphere);

    // Create a point light
    std::vector<std::shared_ptr<Light>> lights;
    auto pointLight = std::make_shared({0, 0, 3}, {1, 1, 1});
    lights.emplace_back(pointLight);

    return Scene(std::move(world), std::move(lights));

}

Camera setupCamera() {
    // Create camera "eyes"
    std::vector<Vertex3f> eyes(2);
    eyes[0] = Vertex3f(-2.0, 0, 0);
    eyes[1] = Vertex3f(-0.5, 0, 0);

    return Camera(eyes, 1000, 1000, 1);
}

int main() {

    auto scene = setupScene();
    auto camera = setupCamera();

    // Render the scene
    camera.render(scene);

    // Save an image to out.png
    camera.createImage("out.png");

    return 0;
}