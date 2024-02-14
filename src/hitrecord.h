#include "material.h"
#include <eigen3/Eigen/Dense>
using namespace std;
using namespace Eigen;

struct HitRecord
{
    // Idea for hitrecord comes from "Fundamentals of Computer Graphics" 5th Edition
    float t;
    Vector3f normal;
    Vector3f origin;
    Vector3f direction;
    Vector3f phit;
    Material mat;
};