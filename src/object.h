#include "hitrecord.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

class Object
{
public:
    HitRecord hit;
    Material mat;
    virtual HitRecord intersect(Vector3f &origin, Vector3f &direction, int depth) = 0;
    virtual ~Object(){};
};

class Sphere : public Object
{
    Vector3f center;
    float radius2;
    bool solveQuadratic(const float &a, const float &b, const float &c, float &t0, float &t1)
    {
        // Code paraphrased from https://www.scratchapixel.com/index.html
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0)
            return false;
        else if (discriminant == 0)
        {
            t0 = t1 = -0.5 * b / a;
        }
        else
        {
            float q = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
            t0 = q / a;
            t1 = c / q;
        }
        if (t0 > t1)
            std::swap(t0, t1);
        return true;
    }

public:
    Sphere(Vector3f center, float radius2, Material mat)
    {
        this->center = center;
        this->radius2 = radius2;
        this->mat = mat;
    }
    HitRecord intersect(Vector3f &origin, Vector3f &direction, int depth)
    {
        // sphere intersection code paraphrased from https://www.scratchapixel.com/index.html
        Vector3f L = origin - center;
        float a = direction.dot(direction);
        float b = 2 * direction.dot(L);
        float c = L.dot(L) - radius2;
        float t0, t1;
        HitRecord hit;
        hit.origin = origin;
        hit.direction = direction;
        if (solveQuadratic(a, b, c, t0, t1))
        {
            hit.t = t0 < t1 ? t0 : t1;
            Vector3f Phit = origin + direction * hit.t;
            hit.phit = Phit;
            Vector3f hitNormal = Phit - this->center;
            hitNormal.normalize();
            hit.normal = hitNormal;
            if (depth < 0)
            {
                return this->intersect(origin, direction, depth + 1);
            }
            return hit;
        }
        hit.t = 100000;
        return hit;
    }
};

class Triangle : public Object
{
    vector<Vector3f> vertices;

public:
    Triangle(vector<Vector3f> vertices, Material mat)
    {
        this->vertices = vertices;
        this->mat = mat;
    }
    HitRecord intersect(Vector3f &origin, Vector3f &direction, int depth)
    {
        // Triangle intersection code paraphrased from https://www.scratchapixel.com/index.html
        // and from "Fundamentals of Computer Graphics" 5th Edition by Steve Marschner and
        // Peter Shirley
        // compute planes normal
        Vector3f v0v1 = this->vertices[1] - this->vertices[0];
        Vector3f v0v2 = this->vertices[2] - this->vertices[0];
        Vector3f normal = v0v1.cross(v0v2);
        normal.normalize();
        HitRecord hit;
        hit.direction = direction;
        hit.origin = origin;
        hit.normal = normal;
        hit.t = 100000;
        Vector3f pvec = direction.cross(v0v2);
        float det = v0v1.dot(pvec);

        if (fabs(det) < 1e-6)
        {
            return hit;
        }

        float invDet = 1 / det;
        Vector3f tvec = origin - this->vertices[0];
        float u = tvec.dot(pvec) * invDet;

        if (u < 0 || u > 1)
            return hit;

        Vector3f qvec = tvec.cross(v0v1);
        float v = direction.dot(qvec) * invDet;
        if (v < 0 || u + v > 1)
            return hit;
        hit.t = v0v2.dot(qvec) * invDet;
        if (depth < 0)
        {
            return this->intersect(origin, direction, depth + 1);
        }
        return hit;
    }
};

class Plane : public Object
{
    Vector3f normal;
    Vector3f p0;

public:
    Plane(Vector3f normal, Vector3f p0, Material mat)
    {
        this->normal = normal;
        this->p0 = p0;
        this->mat = mat;
        normal.normalize();
    }

    HitRecord intersect(Vector3f &origin, Vector3f &direction, int depth)
    {
        // Plane intersection code paraphrased from https://www.scratchapixel.com/index.html
        float denom = this->normal.dot(direction);
        HitRecord hit;
        hit.t = 100000;
        hit.direction = direction;
        hit.normal = this->normal;
        hit.origin = origin;
        if (fabs(denom) > 1e-6)
        {
            Vector3f point = this->p0 - origin;
            hit.t = point.dot(this->normal) / denom;
        }
        if (hit.t < 0)
        {
            hit.t = 100000;
        }
        if (depth < 0)
        {
            return this->intersect(origin, direction, depth + 1);
        }
        return hit;
    }
};