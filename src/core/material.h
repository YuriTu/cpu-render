#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "utils.h"
#include "interaction.h"
// handle bsdf eg

namespace r
{

enum MaterialType { 
    DIFFUSE,
    REFLECTION,
    REFLECTION_AND_REFRACTION,
    TRANSMISSION,
    REFLECTION_TRANSMISSION,
    
};

class Material{
    public:
        Material(){};
        Material(MaterialType t, Vector3f e):m_type(t),m_emission(e) {};
    private:
        Vector3f toWorld(const Vector3f &a,const Vector3f &N) const;

    public:
        MaterialType m_type;
        Vector3f m_emission;
        float ior;
        Vector3f Kd, Ks;
        float specularExponent;
        MaterialType getType();
        Vector3f getEmission() const;
        bool hasEmission() const;
        void setKd(const Vector3f &v);

        // sample a ray by Material properties
        Vector3f sample(const Vector3f &wi, const Vector3f &N, float &pdf) const;

        // given a ray, calculate the contribution of this ray
        inline Vector3f eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

        void ComputeScatteringFunction(SurfaceInteraction *isect) const ;
};



}
#endif //RT_MATERIAL_H
