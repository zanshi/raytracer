//
// Created by Niclas Olmenius on 2017-09-07.
//
#include "common.h"

#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "sphere.h"

using namespace rays;


std::vector<SceneObject> createWorld() {

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

    std::vector<Vertex3f> vertices{v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11};

    // Transfer ownership of vertices to the triangle mesh
    auto mesh = std::make_shared<TriangleMesh>(std::move(vertices));

    // Connect everything
    // Floor
    const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle(mesh, {0, 1, 5}));
    const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle(mesh, {1, 2, 5}));
    const std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(Triangle(mesh, {2, 4, 5}));
    const std::shared_ptr<Shape> t3 = std::make_shared<Triangle>(Triangle(mesh, {2, 3, 4}));

    // Ceiling
    const std::shared_ptr<Shape> t4 = std::make_shared<Triangle>(Triangle(mesh, {7, 6, 11}));
    const std::shared_ptr<Shape> t5 = std::make_shared<Triangle>(Triangle(mesh, {7, 11, 8}));
    const std::shared_ptr<Shape> t6 = std::make_shared<Triangle>(Triangle(mesh, {8, 11, 10}));
    const std::shared_ptr<Shape> t7 = std::make_shared<Triangle>(Triangle(mesh, {8, 10, 9}));

    const std::shared_ptr<Shape> t8 = std::make_shared<Triangle>(Triangle(mesh, {0, 6, 1}));
    const std::shared_ptr<Shape> t9 = std::make_shared<Triangle>(Triangle(mesh, {1, 6, 7}));
    const std::shared_ptr<Shape> t10 = std::make_shared<Triangle>(Triangle(mesh, {1, 7, 2}));

    // -y wall
    const std::shared_ptr<Shape> t11 = std::make_shared<Triangle>(Triangle(mesh, {2, 7, 8}));
    const std::shared_ptr<Shape> t12 = std::make_shared<Triangle>(Triangle(mesh, {2, 8, 3}));

    const std::shared_ptr<Shape> t13 = std::make_shared<Triangle>(Triangle(mesh, {3, 8, 9}));
    const std::shared_ptr<Shape> t14 = std::make_shared<Triangle>(Triangle(mesh, {0, 5, 11}));
    const std::shared_ptr<Shape> t15 = std::make_shared<Triangle>(Triangle(mesh, {0, 11, 6}));

    // +y wall
    const std::shared_ptr<Shape> t16 = std::make_shared<Triangle>(Triangle(mesh, {5, 4, 10}));
    const std::shared_ptr<Shape> t17 = std::make_shared<Triangle>(Triangle(mesh, {5, 10, 11}));

    const std::shared_ptr<Shape> t18 = std::make_shared<Triangle>(Triangle(mesh, {4, 3, 9}));
    const std::shared_ptr<Shape> t19 = std::make_shared<Triangle>(Triangle(mesh, {4, 9, 10}));

    ColorDbl lightred{0.5, 0.0, 0.0};
    ColorDbl lightgreen{0.0, 0.5, 0.0};
    ColorDbl lightblue{0.0, 0.0, 0.5};
    ColorDbl white{1.0, 1.0, 1.0};
    ColorDbl yellow{0.8, 0.8, 0.0};
    ColorDbl magenta{0.8, 0.0, 0.8};
    ColorDbl cyan{0.0, 0.8, 0.8};

    const std::shared_ptr<BSDF> surface_white = std::make_shared<Lambertian>(white);
    const std::shared_ptr<BSDF> surface_red = std::make_shared<Lambertian>(lightred);
    const std::shared_ptr<BSDF> surface_green = std::make_shared<Lambertian>(lightgreen);
    const std::shared_ptr<BSDF> surface_lightblue = std::make_shared<Lambertian>(lightblue);
    const std::shared_ptr<BSDF> surface_yellow = std::make_shared<Lambertian>(yellow);
    const std::shared_ptr<BSDF> surface_magenta = std::make_shared<Lambertian>(magenta);
    const std::shared_ptr<BSDF> surface_cyan = std::make_shared<Lambertian>(cyan);

    std::vector<SceneObject> objects;

    auto light0 = std::make_shared<AreaLight>(AreaLight(white, t8, 10));

    objects.emplace_back(t0, surface_white);
    objects.emplace_back(t1, surface_white);
    objects.emplace_back(t2, surface_white);
    objects.emplace_back(t3, surface_white);

    objects.emplace_back(t4, surface_white);
    objects.emplace_back(t5, surface_white);
    objects.emplace_back(t6, surface_white);
    objects.emplace_back(t7, surface_white);

    objects.emplace_back(t8, surface_magenta);
    objects.emplace_back(t9, surface_magenta);

    objects.emplace_back(t10, surface_green);
    objects.emplace_back(t11, surface_green);

    objects.emplace_back(t12, surface_lightblue);
    objects.emplace_back(t13, surface_lightblue);

    objects.emplace_back(t14, surface_yellow);
    objects.emplace_back(t15, surface_yellow);

    objects.emplace_back(t16, surface_red);
    objects.emplace_back(t17, surface_red);

    objects.emplace_back(t18, surface_cyan);
    objects.emplace_back(t19, surface_cyan);

    return objects;
}

auto createTetrahedron() {
    std::vector<SceneObject> tetrahedron;

    // Define vertices
    Vertex3f v0{7.0f, 4.8f, -3.0f};
    Vertex3f v1{7.0f, -4.8f, -3.0f};
    Vertex3f v2{3.5f, 0.0f, -4.5f};
    Vertex3f v3{5.0f, 0.0f, 0.0f};

    std::vector<Vertex3f> vertices{v0, v1, v2, v3};

    // Define triangles
    const auto mesh = std::make_shared<TriangleMesh>(std::move(vertices));
    const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle(mesh, {0, 3, 1}));
    const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle(mesh, {1, 3, 2}));
    const std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(Triangle(mesh, {2, 3, 0}));
    const std::shared_ptr<Shape> t3 = std::make_shared<Triangle>(Triangle(mesh, {0, 1, 2}));

    // Define surface
    const ColorDbl red{0.8, 0.2, 0.2};
    const ColorDbl white{0.8, 0.8, 0.8};
    const std::shared_ptr<BSDF> surface = std::make_shared<Lambertian>(white);

    // Create and add scene objects
    tetrahedron.emplace_back(t0, surface);
    tetrahedron.emplace_back(t1, surface);
    tetrahedron.emplace_back(t2, surface);
    tetrahedron.emplace_back(t3, surface);

    return tetrahedron;

}


auto createAreaLights() {

    std::vector<SceneObject> lightTriangles;

    Vertex3f v0{5.0f, 2.0f, 4.99f};
    Vertex3f v1{7.0f, 2.0f, 4.99f};
    Vertex3f v2{7.0f, -2.0f, 4.99f};
    Vertex3f v3{5.0f, -2.0f, 4.99f};
    std::vector<Vertex3f> vertices{v0, v1, v2, v3};

    const auto mesh = std::make_shared<TriangleMesh>(std::move(vertices));

    Vector3f normal{0, 0, 1};
    const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle(mesh, {0, 1, 2}));
    const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle(mesh, {2, 3, 0}));

    ColorDbl white{1.0, 1.0, 1.0};
    float intensity = 40;

    const std::shared_ptr<BSDF> surface = std::make_shared<Lambertian>(white);

    std::vector<std::shared_ptr<AreaLight>> areaLights;

    areaLights.emplace_back(std::make_shared<AreaLight>(white, t0, intensity));
    areaLights.emplace_back(std::make_shared<AreaLight>(white, t1, intensity));

    lightTriangles.emplace_back(t0, surface, areaLights[0]);
    lightTriangles.emplace_back(t1, surface, areaLights[1]);

    return std::make_tuple(lightTriangles, areaLights);

}


Scene setupScene() {
    // Create scene
    auto world = createWorld();
    // Add a sphere
    ColorDbl red{0.8, 0.1, 0.1};
    ColorDbl white{1, 1, 1};
    const std::shared_ptr<BSDF> glass_material = std::make_shared<Glass>(white, 1.52f);
    const std::shared_ptr<BSDF> diffuse_material = std::make_shared<OrenNayar>(red, 0.3f);
    auto sphere = SceneObject(std::make_shared<Sphere>(Sphere({4.3f, 2.8f, 1.0f}, 2.2f)),
                              glass_material);
    auto sphere2 = SceneObject(std::make_shared<Sphere>(Sphere({4.0f, -2.2f, 1.5}, 2.0f)),
                               diffuse_material);
    world.push_back(sphere);
    world.push_back(sphere2);

    // Add the tetrahedron to the scene
    auto tetrahedron = createTetrahedron();
    world.insert(end(world), std::make_move_iterator(begin(tetrahedron)), std::make_move_iterator(end(tetrahedron)));

    // Add area lights
    auto[areaLightTris, lights] = createAreaLights();
    world.insert(end(world), std::make_move_iterator(begin(areaLightTris)),
                 std::make_move_iterator(end(areaLightTris)));

//    std::vector<std::shared_ptr<AreaLight>> lights;
//    for (const auto &object : world) {
//        if (object.getAreaLight() != nullptr) {
//            lights.emplace_back(object.areaLight);
//        }
//    }

    return Scene(std::move(world), std::move(lights));

}

Camera setupCamera() {
    // Create camera "eyes"
    std::vector<Vertex3f> eyes(2);
    eyes[0] = Vertex3f(-2.0f, 0.f, 0.f);
    eyes[1] = Vertex3f(-0.5f, 0.f, 0.f);

    return Camera(eyes, 256, 256, 1, 32);
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