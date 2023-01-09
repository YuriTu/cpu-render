#include "tracingRender.h"
#include "utils.h"

#include <omp.h>

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
void r::TracingRender::add(Light &l) {
    lights.push_back(l);
}

Interaction r::TracingRender::castRay(Ray &ray) {
    // ray 
    Interaction ret;
    float t = -1;
    float index = 0;
    for (int i = 0; i < objects.size(); i++) {
        bool flag = objects[i].intersect(ray, t);
        if (flag) {
            index = i;
        }
    }

    if (t < 0) {
        ret.flag = false;
        return ret;
    }
    ret.flag = true;
    ret.hitPoint = ray.o + ray.dir * t;
    ret.hitObjectIndex = index;
    return ret;
}

Vector4f r::TracingRender::getRadiance(Ray &ray) {
    Interaction interaction = castRay(ray);
    if (!interaction.flag) {
        // background
        return background;
    }

    // 1. mode witte style
    // 1. phone style
    // specular diffuse 

    // kd * color * I/r^2 * cos(light, hitpot_N)
    // + ks * color * I/r^2 cos(half ^ N) ^ coeffi
    Vector4f ret(0,0,0);
    Vector4f hitPoint = interaction.hitPoint;

    for (r::Light light : lights) {
        Vector4f lightDir = hitPoint - light.pos;
        float lightDistance2 = lightDir.dot(lightDir);
        lightDir = normalize(lightDir);

        r::Sphere hitOBject = objects[interaction.hitObjectIndex];
        Vector4f N;
        hitOBject.getSurfaceProperties(hitPoint, N);
        Vector4f outgoingDir = reflect(lightDir, N);
        Vector4f halfVector = normalize(outgoingDir + lightDir);

        Ray shadowRay(hitPoint, lightDir);
        Interaction shadowRayInteraction = castRay(shadowRay);
        float diffuseCoefficient = 0;
        if (shadowRayInteraction.flag) {
            Vector4f tempShadowRayDir = shadowRay.o - shadowRayInteraction.hitPoint;
            float tempShandowRayDistance2 = tempShadowRayDir.dot(tempShadowRayDir);
            if (tempShandowRayDistance2 < lightDistance2) {
                // shadow without diffuse
                diffuseCoefficient = 0;
            } else {
                diffuseCoefficient = lightDir.dot(N);
            }
        }

        Vector4f specularColor = light.intensity * std::pow(halfVector.dot(N),hitOBject.specularExponent); 


        Vector4f diffuseRadiance = hitOBject.kd * hitOBject.diffuseColor * diffuseCoefficient;
        Vector4f specularRadiance = hitOBject.ks * specularColor;
        ret += diffuseRadiance + specularRadiance;
    }


    return Vector4f(1,0,0);

    // return radiance
}

void r::TracingRender::render()
{
    int samples = 1;

    Vector4f cam(0,0,10);
    float imageRadio = width / height;
    #pragma omp parallel
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