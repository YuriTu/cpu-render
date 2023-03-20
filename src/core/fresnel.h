#ifndef RT_FRESNEL_H
#define RT_FRESNEL_H

#include "r.h"
#include "geometry.h"

namespace r 
{

float FrDielectric(float cosThetaI, float etaI, float etaT);

inline bool SameHemisphere(const Vector3f &wi, const Vector3f &wo) {
    return wi.z * wo.z > 0;
}
inline float CosTheta(const Vector3f &w) {return w.z;}
inline float AbsCosTheta(const Vector3f &w) {return std::abs(w.z);}


class Fresnel {
public:
    virtual ~Fresnel(){};
    virtual Vector3f evaluate(float cosI) const = 0;
};

class FresnelDielectric : public Fresnel {
public:
    Vector3f evaluate(float cosThetaI) const;
    FresnelDielectric(float etaI, float etaT) : etaI(etaI), etaT(etaT) {};
private:
    float etaI, etaT;
};

class BxDF {
public:
    virtual ~BxDF(){}
    virtual Vector3f Sample_f(const Vector3f &wo, Vector3f *wi, float *pdf) const;
    virtual Vector3f f(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual float Pdf(const Vector3f &wo, const Vector3f &wi) const;
};

class FresnelSpecular : public BxDF {

public:
    FresnelSpecular(const Vector3f &R, const Vector3f &T, float etaI, float etaT)
        :R(R),T(T), etaI(etaI), etaT(etaT) {}
    Vector3f f(const Vector3f &wo, const Vector3f &wi) const;
    Vector3f Sample_f(const Vector3f &wo, Vector3f *wi, float *pdf) const;
    float Pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const Vector3f R,T; // re color \ tau
    const float etaI,etaT;
};

class LambertianReflection : public BxDF {

public:
    LambertianReflection(const Vector3f &R) : R(R){}
    Vector3f f(const Vector3f &wo, const Vector3f &wi) const;

private:
    const Vector3f R;

};

}


#endif