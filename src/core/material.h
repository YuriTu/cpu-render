#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "geometry.h"
#include "utils.h"
// handle bsdf eg

enum MaterialType { DIFFUSE};

class Material{
    public:
        Material(){};
        Material(MaterialType t, Vector3f e):m_type(t),m_emission(e) {};
    private:
        Vector3f toWorld(const Vector3f &a,const Vector3f &N){
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

    public:
        MaterialType m_type;
        Vector3f m_emission;
        float ior;
        Vector3f Kd, Ks;
        float specularExponent;
        //Texture tex;
        MaterialType getType() {return m_type;};
        inline Vector3f getColorAt(double u, double v);
        Vector3f getEmission() {return m_emission;};
        inline bool hasEmission();

        // sample a ray by Material properties
        inline Vector3f sample(const Vector3f &wi, const Vector3f &N);
        // given a ray, calculate the PdF of this ray
        inline float pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);
        // given a ray, calculate the contribution of this ray
        inline Vector3f eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);
};


bool Material::hasEmission() {
    if (m_emission.lengthSquared() > EPSILON) return true;
    else return false;
}

Vector3f Material::getColorAt(double u, double v) {
    return Vector3f();
}


Vector3f Material::sample(const Vector3f &wi, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // uniform sample on the hemisphere
            float x_1 = getRandom(), x_2 = getRandom();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            return toWorld(localRay, N);
            
            break;
        }
    }
}

float Material::pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // uniform sample probability 1 / (2 * PI)
            if (Dot(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
    }
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

#endif //RT_MATERIAL_H
