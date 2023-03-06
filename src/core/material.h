#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "geometry.h"
#include "utils.h"
// handle bsdf eg

namespace r
{

enum MaterialType { 
    DIFFUSE,
    REFLECTION,
    REFLECTION_AND_REFRACTION
};

class Material{
    public:
        Material(){};
        Material(MaterialType t, Vector3f e):m_type(t),m_emission(e) {};
    private:
        Vector3f toWorld(const Vector3f &a,const Vector3f &N);

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



}
#endif //RT_MATERIAL_H
