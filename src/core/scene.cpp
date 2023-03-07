#include "scene.h"

namespace r
{
Scene::Scene(int w, int h) {
    width = w;
    height = h;
    buildBVH();
}

void Scene::add(Mesh * obj) {
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
}

void Scene::buildBVH() {
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

bool intersect(const Ray& ray) {
    return true;
}

}