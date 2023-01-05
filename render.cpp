#include <iostream>
#include "render.h"
#include "utils.h"

r::Render::Render(int w, int h): width(w), height(h) {
    frameBuffer.resize(width * height);
    depthBuffer.resize(width * height);
}

void r::Render::render() 
{
    setMvp();
    rasterization();
    exportImg();
}

void r::Render::setModule(Vector4f position)
{
    m.setPos(position);
}

void r::Render::setView(Vector4f position)
{
    v.setEyePos(position);
}

void r::Render::setPerspectiveProjection(float near, float far, float aspect, float fov)
{
    Matrix4x4 ortho;
    Matrix4x4 orthoTranslate;
    Matrix4x4 orthoScale;
    Matrix4x4 orthoToPerspect;

    float depth = near - far;
    float rad = (fov / 2.0 ) / 180.0 * PI;
    float halfHeight = std::tan(rad) * -near;
    float t = halfHeight;
    float b = -t;
    float l = aspect * t;
    float r = -l;


    float trans[4][4] = {
        {1.0, 0.0,0.0,-(l+r) /2},
        {0.0, 0.0,0.0,-(t+b) /2},
        {0.0, 0.0,0.0,-(near+far) /2},
        {0.0, 0.0,0.0,1.0},
    };

    memcpy(orthoTranslate.m, trans, sizeof(float) * 16);

    float scale[4][4] = {
        {2/ (r-l), 0.0,0.0,0.0},
        {0.0, 2/ (t - b),0.0,0.0},
        {0.0, 0.0,2 / (far - near),0.0},
        {0.0, 0.0,0.0,1.0},
    };

    memcpy(orthoScale.m, scale, sizeof(float) * 16);

    ortho = orthoScale * orthoTranslate;

    float o2p[4][4] = {
        {near, 0.0,0.0,0.0},
        {0.0, near,0.0,0.0},
        {0.0, 0.0,near + far,-(near * far)},
        {0.0, 0.0,near,0.0},
    };

    memcpy(orthoToPerspect.m, o2p, sizeof(float) * 16);

    p = ortho * orthoToPerspect;
}

void r::Render::setMvp()
{
    Matrix4x4 mvp = m * v * p;
    // std::vector<Vector4f> localPoints;
    // for (int i = 0; i < points.size(); i++) {
    //     localPoints.push_back(mvp * points[i]);
    // }
}

void r::Render::add(triangle &m){
    points = m.getData();
    colors = m.getColor();
    std::cout << "points data:" << points.size() << std::endl;
}

void r::Render::exportImg()
{
    FILE* fp = fopen("img.ppm","wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (int i = 0; i < frameBuffer.size(); i++) {
        static unsigned char color[3];
        
        Vector4f pixel = frameBuffer[i];
        color[0] = (char)(255 * clamp(1.0,0., pixel.x));
        color[1] = (char)(255 * clamp(1.0,0., pixel.y));
        color[2] = (char)(255 * clamp(1.0,0., pixel.z));
        // if (pixel.x > 0 || pixel.y > 0) {
        //     std::cout << " frame: x:" << pixel.x << "y:" << pixel.y << 255.0 * clamp(1.0,0., pixel.x) << std::endl;
        //     std::cout << " frame: index:" << i << std::endl;
        //     std::cout << " color: x:" << color[0] << "y:" << color[1] << std::endl;
        // }
        
        fwrite(color, 1, 3, fp);
    }

    fclose(fp);
}

void r::Render::rasterization()
{
    Vector4f defaultColor = Vector4f(1.0,1.0,1.0);
    float x_min = 0;
    float x_max = 0;
    float y_min = 0;
    float y_max = 0;
    for (int i = 0; i < points.size(); i++) {
        Vector4f point = points[i];
        x_min = std::min(x_min, point.x);
        x_max = std::max(x_max, point.x);
        y_min = std::min(y_min, point.y);
        y_max = std::max(y_max, point.y);
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++){
            bool flag = false;
            int index = (height - j) * width + i;
            // aabb judge
            if (i < x_max && i > x_min && j < y_max && j > y_min) {
                if (inTrangle(i,j, points)) {
                    flag = true;
                    // (frameBuffer[index]) = defaultColor;
                }
            }

            if (flag) {
                // compute 重力坐标
                std::vector<float> bray = getBarycentric2D(i,j,points);
                float a = bray[0];
                float b = bray[1];
                float g = bray[2];
                // cal z value 
                float cur_z = points[0].z * a + points[1].z * a + points[2].z * g;
                // judge depth buffer
                if (depthBuffer[index] - cur_z >= .0001) {
                    depthBuffer[index] = cur_z;
                    // get color
                    // Vector4f color = ( colors[0] * a + colors[1] * b + colors[2] * g);
                    Vector4f tem1 = colors[0] * a;
                    Vector4f tem2 = colors[1] * b;
                    Vector4f tem3 = colors[2] * g;
                    Vector4f color = tem1 + tem2 + tem3;
                    frameBuffer[index] = color;
                }
            }
        }
    }
}