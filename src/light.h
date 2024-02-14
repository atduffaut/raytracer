#include "object.h"
#include <iostream>
#include <algorithm>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

class Light
{
public:
    virtual void illuminate(HitRecord hit, Object *object, vector<unsigned char> &color) = 0;
    virtual ~Light(){};
    vector<unsigned char> color;
    float intensity;
};

class DistantLight : public Light
{
    Vector3f direction;

public:
    DistantLight(Vector3f &direction, vector<unsigned char> color = {255, 255, 255}, float intensity = 1.0f)
    {
        this->direction = direction;
        this->color = color;
        this->intensity = intensity;
    }
    void illuminate(HitRecord hit, Object *object, vector<unsigned char> &color)
    {
        // Code paraphrased from "Fundamentals of Computer Graphics" 5th Edition
        if (hit.t >= 100000)
            return;
        vector<unsigned char> tempColor = object->mat.cd;
        float nhit = hit.normal.dot(-1 * this->direction);
        float factor = this->intensity * max(0.0f, nhit) * object->mat.kd;
        for (int i = 0; i < 3; i++)
        {
            if (color[i] + tempColor[i] * factor > 255)
            {
                color[i] = 255;
                continue;
            }
            color[i] += tempColor[i] * factor;
        }
    }
};

class PointLight : public Light
{
    Vector3f point;

public:
    PointLight(Vector3f &point, float intensity = 1.0f, vector<unsigned char> color = {255, 255, 255})
    {
        this->point = point;
        this->color = color;
        this->intensity = intensity;
    }
    void illuminate(HitRecord hit, Object *object, vector<unsigned char> &color)
    {
        // Code paraphrased from https://www.scratchapixel.com/index.html
        if (hit.t >= 100000)
            return;
        vector<unsigned char> tempColor = object->mat.cs;
        Vector3f Phit = hit.phit;
        Vector3f L = this->point - Phit;
        L.normalize();
        Vector3f reflection = 2 * (hit.normal.dot(L)) * hit.normal - L;
        Vector3f dirNorm = hit.direction.normalized();
        float nhit = reflection.dot(-1 * dirNorm);
        nhit = max(0.0f, nhit);

        float factor = object->mat.ks * this->intensity * pow(nhit, 16);

        for (int i = 0; i < 3; i++)
        {
            if (color[i] + tempColor[i] * factor > 255)
            {
                color[i] = 255;
                continue;
            }
            color[i] += tempColor[i] * factor;
        }
    }
};

class AmbientLight : public Light
{
public:
    AmbientLight(float intensity = 1.0f)
    {
        this->intensity = intensity;
    }
    void illuminate(HitRecord hit, Object *object, vector<unsigned char> &color)
    {
        // This code I wrote myself, pretty simple!
        if (hit.t >= 100000)
            return;
        vector<unsigned char> tempColor = object->mat.ca;
        float factor = this->intensity * object->mat.ka;
        for (int i = 0; i < 3; i++)
        {
            if (color[i] + tempColor[i] * factor > 255)
            {
                color[i] = 255;
                continue;
            }
            color[i] += tempColor[i] * factor;
        }
    }
};