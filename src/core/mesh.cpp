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
        interaction->mediumInterface = mediumInterface;
    } else {
        interaction->mediumInterface = MediumInterface(ray.medium);
    }
    return hit;
}

const Material* GeometricPrimitive::getMaterial() const {
    return material.get();
}

void GeometricPrimitive::ComputeScatteringFunction(SurfaceInteraction *isect) const {
    if (material) {
        material->ComputeScatteringFunction(isect);
    }
}

float GeometricPrimitive::Pdf(const Interaction &ref, const Vector3f &wi) const {
    return shape->Pdf(ref, wi);
};

void GeometricPrimitive::Sample(SurfaceInteraction &isect, float &pdf) {
    shape->Sample(isect,pdf);
    isect.primitive = this;
};

float GeometricPrimitive::getArea() const {
    return shape->getArea();
};

}