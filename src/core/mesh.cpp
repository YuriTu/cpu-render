#include "mesh.h"
#include "utils.h"

namespace r{

const int channel = 3;


// Sphere::Sphere(Vector3f _o, float _r, Vector3f _c, utils::reflectType _t):o(_o), radius(_r), color(_c), reflectType(_t) {
//     diffuseColor = _c;
//     kd = 0.8;
//     ks = 0.2;
//     specularExponent = 25;
//     ior = 0.8;
//     radius2 = radius * radius;
//     // area = 4 * PI * radius2;
//     area = 2 * PI;
// };

// 单点算相交
// todo: float的比较精度
// bool Sphere::intersect(Ray &r, float &tNear) {
//     float a = Dot(r.d,r.d);
//     Vector3f o2o = r.o - o;
//     float b = Dot(r.d * 2,o2o);
//     float c = Dot(o2o,o2o) - (radius * radius);
//     float safeQuad = b*b - 4 * a * c;
//     if (safeQuad < 0) {
//         return false;
//     }
//     float tMax = (-b + std::sqrt(safeQuad)) / 2 * a;
//     float tMin = (-b - std::sqrt(safeQuad)) / 2 * a;
//     if (tMin < 0) {
//         tMin = tMax;
//     }
//     if (tMin < 0) {
//         return false;
//     }
//     tNear = tMin;
//     return true;
// }

// void Sphere::getSurfaceProperties(Vector3f &hitPoint, Vector3f &N) {
//     Vector3f _N = hitPoint - o;
//     N = normalize(_N);
// }

// void Sphere::sampleSphereUniform(Interaction& ret, float& pdf){
//     // Vector3f dir = getVecFromSampleSphereUniform();
//     // ret.hitPoint = this->o + this->radius * dir;
//     // ret.hitObject = this;
//     // ret.emit = this->emit;
//     // ret.normal = dir;
    
//     // pdf = 1.f/ area;
// }

// Vector3f Sphere::evalBRDF(const Vector3f& wo, const Vector3f& N){

//     // kd = 1.f;
//     // return (diffuseColor * kd) / (PI * 2.f);

//     Vector3f ret(0.f);
//     if (reflectType == utils::DIFFUSE) {
//         float cos = std::max(0.f, Dot(wo,N));
//         if (cos > EPS) {
//             kd = 1.f;
//             // fixme我理解这里应该是半球2pi
//             ret = (diffuseColor * kd) / (PI);
//         }
//     }
//     return ret;
// }

// bool Sphere::hasEmit(){
//     return ( Dot(emit, emit) > 0);
// }

}