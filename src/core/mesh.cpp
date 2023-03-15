#include "mesh.h"

namespace r{

Bounds3 GeometricPrimitive::WorldBound() const {
    return shape->WorldBound();
}

bool GeometricPrimitive::intersect(const Ray& ray, SurfaceInteraction *interaction){
    
    bool hit = shape->intersect(ray, interaction);
    interaction->primitive = this;
    // warning
    // ray.setTMax(interaction->distance);

    if (mediumInterface.isMediumTransition()) {

    }
    return hit;
}

const Material* GeometricPrimitive::getMaterial() const {
    return material.get();
}

void GeometricPrimitive::ComputeScatteringFunction(SurfaceInteraction *isect) const {
    material->ComputeScatteringFunction(isect);
}

void GeometricPrimitive::Sample(SurfaceInteraction &isect, float &pdf) {
    shape->Sample(isect,pdf);
    isect.primitive = this;
};

float GeometricPrimitive::getArea() const {
    return shape->getArea();
};

}