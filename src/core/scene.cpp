#include "scene.h"

namespace r
{
Scene::Scene(int w, int h) {
    width = w;
    height = h;
}

void Scene::add(Mesh* const obj) {
    objects.push_back(obj);
}

void Scene::add(std::vector<Mesh *> lists) {
    objects.insert(objects.end(),lists.begin(),lists.end());
}

void Scene::applyTemplate(const SceneBasic temp) {
    fov = temp.fov;
    aspect = temp.aspect;
    samples = temp.sampleCount;
    background = temp.background;
    maxDepth = temp.maxDepth;
    camPos = temp.camPos;
    buildBVH();
}

bool Scene::intersect(const Ray& ray,Interaction *isect ) const {
    return this->bvh->intersect(ray, isect);
};

void Scene::buildBVH() {
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}


}