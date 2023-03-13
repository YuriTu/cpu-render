#include "medium.h"

namespace r {

Vector3f HomogeneousMedium::Tr(const Ray &ray) const {
    return EXP(-sigma_t * std::min(ray.tMax * ray.d.length(), MAXFloat));
}

Vector3f HomogeneousMedium::Sample(const Ray &ray) const {

}

}