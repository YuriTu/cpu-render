#include "material.h"
#include "fresnel.h"

namespace r 
{

Vector3f Material::toWorld(const Vector3f &a,const Vector3f &N) const{
    Vector3f B, C;
    if (std::fabs(N.x) > std::fabs(N.y)){
        float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
        C = Vector3f(N.z * invLen, 0.0f, -N.x *invLen);
    }
    else {
        float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
        C = Vector3f(0.0f, N.z * invLen, -N.y *invLen);
    }
    B = Cross(C, N);
    return a.x * B + a.y * C + a.z * N;
}

bool Material::hasEmission() const {
    if (m_emission.lengthSquared() > EPSILON) return true;
    else return false;
}

Vector3f Material::getEmission() const {
    return this->m_emission;
}

MaterialType Material::getType() {
    return m_type;
}


Vector3f Material::sample(const Vector3f &wi, const Vector3f &N, float &pdf) const{
    pdf = 0;
    Vector3f rs = Vector3f();
    switch(m_type){
        case DIFFUSE:
        {
            // uniform sample on the hemisphere
            float x_1 = getRandom(), x_2 = getRandom();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            // pdf = 1 / 2pi
            pdf = 1.f * Inv2Pi;
            rs = toWorld(localRay, N);
            
            break;
        }
        case REFLECTION_AND_REFRACTION:
        {
            // 比例 pdf wi的方向
        }
        
    }
    return rs;
}

void Material::setKd(const Vector3f &v) {
    this->Kd = v;
}

void Material::ComputeScatteringFunction(SurfaceInteraction *isect) const {
    // todo compute bsdf 
    switch (m_type)
    {
    case DIFFUSE:
        {
            isect->bsdf = new LambertianReflection(this->Kd);
        }
        break;
    
    default:
         isect->bsdf;
         printf("error material");
        break;
    }
}

}