#include "scene.h"

namespace r
{
Scene::Scene(int w, int h) {
    width = w;
    height = h;
}

void Scene::add(std::shared_ptr<Mesh> obj) {
    objects.push_back(obj);
}

void Scene::add(std::vector<std::shared_ptr<Mesh>> lists) {
    objects.insert(objects.end(),lists.begin(),lists.end());
}

void Scene::applyTemplate(const SceneBasic temp) {
    fov = temp.fov;
    aspect = temp.aspect;
    samples = temp.sampleCount;
    background = temp.background;
    maxDepth = temp.maxDepth;
    camPos = temp.camPos;
    rrThreshold = temp.rrThreshold;
    buildBVH();
    _initLightList();
}

void Scene::_initLightList() {
    int nLight = 0;
    for (size_t i = 0; i < this->objects.size(); i++) {
        Material* material = objects[i]->getMaterial();
        if (material->hasEmission()) {
            lights.push_back(objects[i]);
        }
    }
}

bool Scene::intersect(const Ray& ray,SurfaceInteraction *isect ) const {
    return this->bvh->intersect(ray, isect);
};

void Scene::buildBVH() {
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}


}