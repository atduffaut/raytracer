#include <iostream>
#include <algorithm>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

class Camera
{
    Vector3f from;
    Vector3f to;
    Vector3f up;
    Matrix4f cameraToWorld;
    bool perspective;

    void lookAtVector(Vector3f &from, Vector3f &to, Vector3f &up, Matrix4f &m)
    {

        // Code to calculate LookAt Vector comes from https://www.scratchapixel.com/index.html
        Vector3f forward = from - to;
        forward.normalize();
        Vector3f right = up.cross(forward);
        // right.normalize();
        Vector3f newUp = forward.cross(right);

        // create the transformation matrix
        m(0, 0) = right[0], m(0, 1) = right[1], m(0, 2) = right[2], m(0, 3) = 0;
        m(1, 0) = newUp[0], m(1, 1) = newUp[1], m(1, 2) = newUp[2], m(1, 3) = 0;
        m(2, 0) = forward[0], m(2, 1) = forward[1], m(2, 2) = forward[2], m(2, 3) = 0;
        m(3, 0) = from[0], m(3, 1) = from[1], m(3, 2) = from[2], m(3, 3) = 1;
    }

public:
    Camera(bool perspective = true)
    {
        this->perspective = perspective;
        from << 0, 0, 0;
        to << 0, 0, -1;
        up << 0, 1, 0;
        lookAtVector(from, to, up, cameraToWorld);
    }
    Camera(Vector3f from, Vector3f to, bool perspective = true)
    {
        this->from = from;
        this->to = to;
        up << 0, 1, 0;
        this->perspective = perspective;
        lookAtVector(from, to, up, cameraToWorld);
    }
    void updateCamera(Vector3f from, Vector3f to, bool perspective = true)
    {
        this->from = from;
        this->to = to;
        this->perspective = perspective;
        lookAtVector(from, to, up, cameraToWorld);
    }
    void getOrigin(float x, float y, Vector3f &origin)
    {
        // Code paraphrased from https://www.scratchapixel.com/index.html
        Matrix4f m = cameraToWorld;
        Vector3f temp = this->from;
        if (!perspective)
        {
            temp[0] += x;
            temp[1] += y;
        }
        float a, b, c, w;
        a = temp[0] * m(0, 0) + temp[1] * m(1, 0) + temp[2] * m(2, 0) + m(3, 0);
        b = temp[0] * m(0, 1) + temp[1] * m(1, 1) + temp[2] * m(2, 1) + m(3, 1);
        c = temp[0] * m(0, 2) + temp[1] * m(1, 2) + temp[2] * m(2, 2) + m(3, 2);
        w = temp[0] * m(0, 3) + temp[1] * m(1, 3) + temp[2] * m(2, 3) + m(3, 3);

        origin[0] = a / w;
        origin[1] = b / w;
        origin[2] = c / w;
    }
    void getDirection(float x, float y, Vector3f &direction)
    {
        // Code paraphrased from https://www.scratchapixel.com/index.html
        Vector3f temp;
        if (!perspective)
        {
            temp << 0, 0, -1;
        }
        else
        {
            temp << x, y, -1;
        }
        Matrix4f m = cameraToWorld;
        float a, b, c;
        a = temp[0] * m(0, 0) + temp[1] * m(1, 0) + temp[2] * m(2, 0);
        b = temp[0] * m(0, 1) + temp[1] * m(1, 1) + temp[2] * m(2, 1);
        c = temp[0] * m(0, 2) + temp[1] * m(1, 2) + temp[2] * m(2, 2);

        direction[0] = a;
        direction[1] = b;
        direction[2] = c;
    }
};