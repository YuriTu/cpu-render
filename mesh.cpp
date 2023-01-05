#include "mesh.h"
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