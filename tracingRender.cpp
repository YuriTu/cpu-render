#include "tracingRender.h"
#include "utils.h"

r::TracingRender::TracingRender(int w, int h): width(w),height(h) {
    frameBuffer.resize(width * height);
    background = Vector4f(0.3);
}

void r::TracingRender::setPerspectiveProjection(float near, float far, float aspect, float fov) {
    // ray dir?
}

void r::TracingRender::setModule(Vector4f position) {
    // ray dir?
}

void r::TracingRender::setView(Vector4f position) {
    // pos lookat updir
    // set camera pos
}

void r::TracingRender::add(Sphere &m) {
    objects.push_back(m);
}

bool r::TracingRender::castRay(Ray &ray) {
    // ray 
    float t = -1;
    float index = 0;
    for (int i; i < objects.size(); i++) {
        float _t = objects[i].intersect(ray);
        if (_t >= 0 && _t < t) {
            t = _t;
            index = i;
        }
    }

    if (t < 0) {
        return false;
    }



    return true;
    

}

Vector4f r::TracingRender::getRadiance(Ray &ray) {

    if (!castRay(ray)) {
        // background
        return background;
    }

    return Vector4f(1,0,0);

    // return radiance
}

void r::TracingRender::render()
{
    int samples = 50;
    // add objects
    // from mvp to get ray

    // loop all pixel 

    // get ray

    // intersect?

    //radiance


    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Vector4f radiance;
            Ray ray(Vector4f(i,j,-1),Vector4f(0,0,0));
            int index = getIndex(i,j,width,height);

            for (int k = 0; k < samples; k++) {
                Vector4f _radiance = getRadiance(ray);
                radiance = radiance + _radiance;
            }
            int s = frameBuffer.size();
            frameBuffer[index] = radiance / samples;
        }
    }

    exportImg(frameBuffer, width,height);
}