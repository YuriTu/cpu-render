#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include "r.h"
#include "geometry.h"
#include "interaction.h"

namespace r 
{

class VisibilityTester {
    public:
        VisibilityTester(){}
        VisibilityTester(const Interaction &p0, const Interaction &p1)
            :p0(p0), p1(p1) {}
        bool isOccluded(const Scene &scene) const;
        Vector3f tr(const Scene &scene) const;
    private:
        Interaction p0, p1;
};

}

#endif