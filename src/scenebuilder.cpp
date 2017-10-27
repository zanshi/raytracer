//
// Created by niclas on 2017-10-26.
//

#include "common.h"
#include "scenebuilder.h"
#include "sceneobject.h"
#include "triangle.h"
#include "sphere.h"
#include "scene.h"

namespace rays {

    auto SceneBuilder::createWorld() {

        //
        glm::vec3 v0(-3.0, 0.0, -5.0);
        glm::vec3 v1(0.0, -6.0, -5.0);
        glm::vec3 v2(10.0, -6.0, -5.0);
        glm::vec3 v3(13.0, 0.0, -5.0);
        glm::vec3 v4(10.0, 6.0, -5.0);
        glm::vec3 v5(0.0, 6.0, -5.0);

        // Ceiling
        glm::vec3 v6(-3.0, 0.0, 5.0);
        glm::vec3 v7(0.0, -6.0, 5.0);
        glm::vec3 v8(10.0, -6.0, 5.0);
        glm::vec3 v9(13.0, 0.0, 5.0);
        glm::vec3 v10(10.0, 6.0, 5.0);
        glm::vec3 v11(0.0, 6.0, 5.0);

        // Connect everything
        // Floor
        const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle({v0, v1, v5}));
        const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle({v1, v2, v5}));
        const std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(Triangle({v2, v4, v5}));
        const std::shared_ptr<Shape> t3 = std::make_shared<Triangle>(Triangle({v2, v3, v4}));

        // Ceiling
        const std::shared_ptr<Shape> t4 = std::make_shared<Triangle>(Triangle({v7, v6, v11}));
        const std::shared_ptr<Shape> t5 = std::make_shared<Triangle>(Triangle({v7, v11, v8}));
        const std::shared_ptr<Shape> t6 = std::make_shared<Triangle>(Triangle({v8, v11, v10}));
        const std::shared_ptr<Shape> t7 = std::make_shared<Triangle>(Triangle({v8, v10, v9}));

        const std::shared_ptr<Shape> t8 = std::make_shared<Triangle>(Triangle({v0, v6, v1}));
        const std::shared_ptr<Shape> t9 = std::make_shared<Triangle>(Triangle({v1, v6, v7}));
        const std::shared_ptr<Shape> t10 = std::make_shared<Triangle>(Triangle({v1, v7, v2}));

        // -y wall
        const std::shared_ptr<Shape> t11 = std::make_shared<Triangle>(Triangle({v2, v7, v8}));
        const std::shared_ptr<Shape> t12 = std::make_shared<Triangle>(Triangle({v2, v8, v3}));

        const std::shared_ptr<Shape> t13 = std::make_shared<Triangle>(Triangle({v3, v8, v9}));
        const std::shared_ptr<Shape> t14 = std::make_shared<Triangle>(Triangle({v0, v5, v11}));
        const std::shared_ptr<Shape> t15 = std::make_shared<Triangle>(Triangle({v0, v11, v6}));

        // +y wall
        const std::shared_ptr<Shape> t16 = std::make_shared<Triangle>(Triangle({v5, v4, v10}));
        const std::shared_ptr<Shape> t17 = std::make_shared<Triangle>(Triangle({v5, v10, v11}));

        const std::shared_ptr<Shape> t18 = std::make_shared<Triangle>(Triangle({v4, v3, v9}));
        const std::shared_ptr<Shape> t19 = std::make_shared<Triangle>(Triangle({v4, v9, v10}));

        ColorDbl lightred{0.5, 0.0, 0.0};
        ColorDbl lightgreen{0.0, 0.5, 0.0};
        ColorDbl lightblue{0.0, 0.0, 0.5};
        ColorDbl white{1.0, 1.0, 1.0};
        ColorDbl yellow{0.7, 0.7, 0.0};
        ColorDbl magenta{0.7, 0.0, 0.7};
        ColorDbl cyan{0.0, 0.7, 0.7};

        const std::shared_ptr<BSDF> surface_white = std::make_shared<Lambertian>(white);
        const std::shared_ptr<BSDF> surface_red = std::make_shared<Lambertian>(lightred);
        const std::shared_ptr<BSDF> surface_green = std::make_shared<Lambertian>(lightgreen);
        const std::shared_ptr<BSDF> surface_lightblue = std::make_shared<Lambertian>(lightblue);
        const std::shared_ptr<BSDF> surface_yellow = std::make_shared<Lambertian>(yellow);
        const std::shared_ptr<BSDF> surface_magenta = std::make_shared<Lambertian>(magenta);
        const std::shared_ptr<BSDF> surface_cyan = std::make_shared<Lambertian>(cyan);

        std::vector<SceneObject> objects;

//        auto light0 = std::make_shared<AreaLight>(AreaLight(white, t14, 6));

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

    auto SceneBuilder::createTetrahedron() {
        std::vector<SceneObject> tetrahedron;

        // Define vertices
        glm::vec3 v0{6.0f, 4.8f, -4.0f};
        glm::vec3 v1{6.0f, -4.8f, -4.0f};
        glm::vec3 v2{3.6f, -1.3f, -1.7f};
        glm::vec3 v3{5.5f, -1.0f, 0.0f};

        // Define triangles
        const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle({v0, v3, v1}));
        const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle({v1, v3, v2}));
        const std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(Triangle({v2, v3, v0}));
        const std::shared_ptr<Shape> t3 = std::make_shared<Triangle>(Triangle({v0, v1, v2}));

        // Define surface
        const ColorDbl white{0.8, 0.8, 0.8};
        const std::shared_ptr<BSDF> surface = std::make_shared<OrenNayar>(white, 0.3f);

        // Create and add scene objects
        tetrahedron.emplace_back(t0, surface);
        tetrahedron.emplace_back(t1, surface);
        tetrahedron.emplace_back(t2, surface);
        tetrahedron.emplace_back(t3, surface);

        return tetrahedron;

    }


    auto SceneBuilder::createAreaLights() {

        std::vector<SceneObject> lightTriangles;

        glm::vec3 v0{4.5f, 0.8f, 4.9999f};
        glm::vec3 v1{5.0f, 0.8f, 4.9999f};
        glm::vec3 v2{5.0f, -0.8f, 4.9999f};
        glm::vec3 v3{4.5f, -0.8f, 4.9999f};

        const std::shared_ptr<Shape> t0 = std::make_shared<Triangle>(Triangle({v0, v1, v2}));
        const std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Triangle({v2, v3, v0}));

        ColorDbl white{1.0, 0.95, 0.95};
        float intensity = 15.0f;

        const std::shared_ptr<BSDF> surface = std::make_shared<Lambertian>(white);

        std::vector<std::shared_ptr<AreaLight>> areaLights;

        areaLights.emplace_back(std::make_shared<AreaLight>(white, t0, intensity));
        areaLights.emplace_back(std::make_shared<AreaLight>(white, t1, intensity));

        lightTriangles.emplace_back(t0, surface, areaLights[0]);
        lightTriangles.emplace_back(t1, surface, areaLights[1]);

        return lightTriangles;

    }

    auto SceneBuilder::createSpheres() {

        std::vector<SceneObject> spheres;
        // Add a sphere
        ColorDbl red{0.8, 0.1, 0.1};
        ColorDbl white{1, 1, 1};

        const std::shared_ptr<BSDF> glass_material = std::make_shared<Glass>(white, 1.52f);
        const std::shared_ptr<BSDF> diffuse_material = std::make_shared<Lambertian>(red);
        const std::shared_ptr<BSDF> mirror_material = std::make_shared<Mirror>(red, 0.3f);
        auto sphere_glass =
                SceneObject(std::make_shared<Sphere>(Sphere({5.0f, 3.0f, 0.0f}, 1.8f)), glass_material);
        auto sphere_diffuse =
                SceneObject(std::make_shared<Sphere>(Sphere({4.5f, -2.7f, 1.0f}, 1.5f)), diffuse_material);
        auto sphere_mirror =
                SceneObject(std::make_shared<Sphere>(Sphere({7.0f, 0.5f, 1.3f}, 1.7f)), mirror_material);
        spheres.push_back(sphere_glass);
        spheres.push_back(sphere_diffuse);
        spheres.push_back(sphere_mirror);

        return spheres;
    }

    void SceneBuilder::addSceneObjects(std::vector<SceneObject> &world, std::vector<SceneObject> &&objectsToAdd) {
        world.insert(end(world), std::make_move_iterator(begin(objectsToAdd)),
                     std::make_move_iterator(end(objectsToAdd)));
    }


    Scene SceneBuilder::setupScene() {
        // Create scene
        auto world = createWorld();

        // Add the tetrahedron to the scene
        addSceneObjects(world, createTetrahedron());

        // Add area light triangles
        addSceneObjects(world, createAreaLights());

        // Add spheres
        addSceneObjects(world, createSpheres());

        // Extract the area lights from the objects
        std::vector<std::shared_ptr<AreaLight>> lights;
        for (const auto &object : world) {
            if (object.getAreaLight() != nullptr) {
                lights.emplace_back(object.areaLight);
            }
        }

        return Scene(world, lights);

    }


}
