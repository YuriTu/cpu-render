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
// 单点算相交
// todo: float的比较精度
float r::Sphere::intersect(Ray &r) {
    float a = r.dir.dot(r.dir);
    Vector4f o2o = r.o - o;
    float b = (r.dir * 2).dot(o2o);
    float c = o2o.dot(o2o) - (radius * radius);
    float tMax = (-b + std::sqrt(b*b - 4 * a * c)) / 2 * a;
    float tMin = (-b - std::sqrt(b*b - 4 * a * c)) / 2 * a;
    if (tMax < 0 && tMin < 0) {
        return tMax;
    }
    return std::min(tMax,tMin);
}