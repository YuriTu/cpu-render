#include "mesh.h"
#include "utils.h"
const int channel = 3;

std::vector<Vector4f> r::triangle::getData(){
    return data;
}

std::vector<Vector4f> r::triangle::getColor(){
    return color;
}

void r::triangle::setColor(Vector4f c1){
    color = {c1, c1, c1};
}

void r::triangle::setColor(Vector4f c1, Vector4f c2, Vector4f c3){
    color = {c1, c2, c3};
}

r::Sphere::Sphere(Vector4f _o, float _r, Vector4f _c, utils::reflectType _t):o(_o), radius(_r), color(_c), reflectType(_t) {
    diffuseColor = _c;
    kd = 0.8;
    ks = 0.2;
    specularExponent = 25;
    ior = 0.8;
    radius2 = radius * radius;
    // area = 4 * PI * radius2;
    area = 2 * PI;
};

// 单点算相交
// todo: float的比较精度
bool r::Sphere::intersect(Ray &r, float &tNear) {
    float a = r.dir.dot(r.dir);
    Vector4f o2o = r.o - o;
    float b = (r.dir * 2).dot(o2o);
    float c = o2o.dot(o2o) - (radius * radius);
    float safeQuad = b*b - 4 * a * c;
    if (safeQuad < 0) {
        return false;
    }
    float tMax = (-b + std::sqrt(safeQuad)) / 2 * a;
    float tMin = (-b - std::sqrt(safeQuad)) / 2 * a;
    if (tMin < 0) {
        tMin = tMax;
    }
    if (tMin < 0) {
        return false;
    }
    tNear = tMin;
    return true;
}

void r::Sphere::getSurfaceProperties(Vector4f &hitPoint, Vector4f &N) {
    Vector4f _N = hitPoint - o;
    N = normalize(_N);
}

void r::Sphere::sampleSphereUniform(Interaction& ret, float& pdf){
    Vector4f dir = getVecFromSampleSphereUniform();
    ret.hitPoint = this->o + this->radius * dir;
    ret.hitObject = this;
    ret.emit = this->emit;
    ret.normal = dir;
    
    pdf = 1.f/ area;
}

Vector4f r::Sphere::evalBRDF(const Vector4f& wo, const Vector4f& N){

    // kd = 1.f;
    // return (diffuseColor * kd) / (PI * 2.f);

    Vector4f ret(0.f);
    if (reflectType == utils::DIFFUSE) {
        float cos = std::max(0.f, wo.dot(N));
        if (cos > EPS) {
            kd = 1.f;
            // fixme我理解这里应该是半球2pi
            ret = (diffuseColor * kd) / (PI);
        }
    }
    return ret;
}

bool r::Sphere::hasEmit(){
    return (emit.dot(emit) > 0);
}