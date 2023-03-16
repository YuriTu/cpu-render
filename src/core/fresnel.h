#ifndef RT_FRESNEL_H
#define RT_FRESNEL_H

#include "r.h"
#include "geometry.h"

namespace r 
{

float FrDielectric(float cosThetaI, float etaI, float etaT);

class Fresnel {
    public:
        virtual ~Fresnel();
        virtual Vector3f evaluate(float cosI) const = 0;

}

}


#endif