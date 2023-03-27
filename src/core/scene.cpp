#include "scene.h"

namespace r
{
Scene::Scene(int w, int h) {
    width = w;
    height = h;
}

void Scene::add(std::shared_ptr<GeometricPrimitive> obj) {
    objects.push_back(obj);
}

void Scene::add(std::vector<std::shared_ptr<GeometricPrimitive>> lists) {
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
        const Material* material = objects[i]->getMaterial();
        if (material->hasEmission()) {
            lights.push_back(objects[i]);
        }
    }
}

bool Scene::intersect(const Ray& ray,SurfaceInteraction *isect ) const {
    return this->bvh->intersect(ray, isect);
};

bool Scene::intersectTr(Ray& ray,SurfaceInteraction *isect, Vector3f *tr) const {
    // 类似visibility的逻辑
    *tr = Vector3f(1.f);
    while (true) {
        bool hitSurface = this->intersect(ray, isect);

        if (ray.medium) {
            *tr *= ray.medium->Tr(ray);
        }

        if (!hitSurface) {
            return false;
        }

        isect->ComputeScatteringFunction(ray);
        if (isect->bsdf != nullptr) {
            return true;
        }

        ray = isect->spawnRay(ray.d);
    }
}

void Scene::buildBVH() {
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}


}