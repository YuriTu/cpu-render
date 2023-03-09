#include "interaction.h"

namespace r 
{

Interaction::Interaction() {
    happened=false;
    coords=Vector3f();
    normal=Vector3f();
    distance= std::numeric_limits<double>::max();
    primitive = nullptr;
    bsdf = Vector3f(0.0);
}

Interaction::Interaction(bool h) {
    happened = h;
}

Vector3f Interaction::Le() const{
    Vector3f rs(0.f);
    if (primitive->material) {
        rs = primitive->material->getEmission();
    }
    return rs;
}

void Interaction::ComputeScatteringFunction(const Ray &ray) {
    return primitive->ComputeScatteringFunction(this);
}


}