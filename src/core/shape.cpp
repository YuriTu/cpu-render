#include "shape.h"
#include "geometry.h"

namespace r {

float Shape::Pdf(const Interaction &ref) const {
    return 1 / getArea();
}

float Shape::Pdf(const Interaction &ref, const Vector3f &wi) const {
    // 看下光线是否相交

    Ray ray = ref.Swapray(wi);
    SurfaceInteraction isectLight;


    if (!intersect(ray, &isectLight)) {
        return 0;
    }
    float distance2 = (ref.p - isectLight.p).lengthSquared();
    float cosTheta = AbsDot(isectLight.n, -wi);

    float pdf = distance2 / cosTheta * getArea();

    if (pdf > 1e4) {
        pdf = 0;
    }

    return pdf;
}

}