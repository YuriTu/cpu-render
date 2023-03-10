#include "material.h"

namespace r 
{

Vector3f Material::toWorld(const Vector3f &a,const Vector3f &N){
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

bool Material::hasEmission() {
    if (m_emission.lengthSquared() > EPSILON) return true;
    else return false;
}

Vector3f Material::getEmission() {
    return this->m_emission;
}

MaterialType Material::getType() {
    return m_type;
}


Vector3f Material::sample(const Vector3f &wi, const Vector3f &N, float &pdf){
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
    }
    return rs;
}


Vector3f Material::eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // calculate the contribution of diffuse   model
            float cosalpha = Dot(N, wo);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = Kd / M_PI;
                return diffuse;
            }
            else
                return Vector3f(0.0f);
            break;
        }
    }
    return Vector3f(0.0);
}

void Material::setKd(const Vector3f &v) {
    this->Kd = v;
}

void Material::ComputeScatteringFunction(Interaction *isect) const {
    // todo compute bsdf 
    isect->bsdf = Vector3f(1.f);
    switch (m_type)
    {
    case DIFFUSE:
        {
            // WI WO n cos integrater 
            float cosTheta = Dot(isect->n, isect->wo);
            if (cosTheta > 0.f) {
                // 半球cos积分pi 非面积积分（2pi）
                isect->bsdf = this->Kd / PI;
            }
        }
        break;
    
    default:
         isect->bsdf;
        break;
    }
}

}