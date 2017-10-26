//
// Created by niclas on 2017-10-26.
//

#ifndef RAYTRACER_SCENEBUILDER_H
#define RAYTRACER_SCENEBUILDER_H


namespace rays {

    class SceneBuilder {
    public:
        Scene setupScene();

    private:
        auto createWorld();

        auto createTetrahedron();

        auto createSpheres();

        auto createAreaLights();

        void addSceneObjects(std::vector<SceneObject> &world, std::vector<SceneObject> &&objectsToAdd);
    };


}

#endif //RAYTRACER_SCENEBUILDER_H
