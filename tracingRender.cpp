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
    for (int i = 0; i < objects.size(); i++) {
        bool flag = objects[i].intersect(ray, t);
        if (flag) {
            index = i;
        }
    }

    if (t < 0) {
        return false;
    }
    return true;
}

Vector4f r::TracingRender::getRadiance(Ray &ray) {
    // printf("start cast");
    if (!castRay(ray)) {
        // background
        return background;
    }
    // printf("has cast!!");
    return Vector4f(1,0,0);

    // return radiance
}

void r::TracingRender::render()
{
    int samples = 1;
    // add objects
    // from mvp to get ray

    // loop all pixel 

    // get ray

    // intersect?

    //radiance
    Vector4f cam(0,0,10);
    float imageRadio = width / height;


    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Vector4f radiance;
            float x = (2.0 * ((i + 0.5f) / width) - 1.0f);
            float y = (2.0 * ((j + 0.5f) / height) - 1.0f)  ;
            Vector4f _dir(x,y,-1);
            Vector4f dir = normalize(_dir);
            Ray ray(cam,dir);
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