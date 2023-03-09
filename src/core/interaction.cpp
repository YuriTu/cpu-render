#include "interaction.h"

namespace r 
{

Interaction::Interaction() {
    happened=false;
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
    primitive = nullptr;
    bsdf = Vector3f(0.0);
}

Interaction::Interaction(bool h) {
    happened = h;
}

Vector3f Interaction::Le() const{
    Vector3f rs(0.f);
    if (primitive->getMaterial()) {
        // todo 这里要考虑一下cos 背面insect light 不能贡献能量
        rs = primitive->getMaterial()->getEmission();
    }
    return rs;
}

void Interaction::ComputeScatteringFunction(const Ray &ray) {
    return primitive->ComputeScatteringFunction(this);
}

Ray Interaction::spawnRay(const Vector3f &d) const {
    Vector3f o = this->p;
    return Ray(o,d);
}


}