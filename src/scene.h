#include "light.h"
#include "camera.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

class Scene
{
    // Vector of unique pointers recommended by https://www.scratchapixel.com/index.html
    vector<unique_ptr<Object>> objects;
    vector<unique_ptr<Light>> lights;
    Camera camera;
    vector<unsigned char> background;
    void getBackground(unsigned char *image, int index)
    {
        image[index] = this->background[0];
        image[index + 1] = this->background[1];
        image[index + 2] = this->background[2];
    }
    void displayScreen(unsigned char *image, int index, vector<unsigned char> color)
    {
        image[index] = color[0];
        image[index + 1] = color[1];
        image[index + 2] = color[2];
    }
    vector<unique_ptr<Object>> buildObjects()
    {
        vector<unique_ptr<Object>> objects;
        Vector3f center1(2, 0.4, -6);
        Vector3f center2(3, 1.1, -11);
        Vector3f normal(0, 1, 0);
        Vector3f p0(0, -2, 0);
        vector<unsigned char> red = {255, 0, 0};
        vector<unsigned char> yellow = {255, 255, 0};
        vector<unsigned char> cyan = {0, 255, 255};
        vector<unsigned char> white = {255, 255, 255};
        Material m1(red, red, white, 0.3, 0.6, 0.3, 0.0);
        Material m2(yellow, yellow, white, 0.3, 0.6, 0.3, 0.0);
        Material m3(cyan, cyan, white, 0.3, 0.6, 0.3, 0.0);
        Material m4(white, white, white, 0.3, 0.4, 0.3, 0.8);

        Vector3f v0(-2, 0, -8);
        Vector3f v1(2, 0, -8);
        Vector3f v2(0, 0, -10);
        vector<Vector3f> vertices1;
        vertices1.push_back(v0);
        vertices1.push_back(v1);
        vertices1.push_back(v2);

        Vector3f v3(0, 3, -9);
        vector<Vector3f> vertices2;
        vertices2.push_back(v3);
        vertices2.push_back(v1);
        vertices2.push_back(v0);

        vector<Vector3f> vertices3;
        vertices3.push_back(v1);
        vertices3.push_back(v2);
        vertices3.push_back(v3);

        vector<Vector3f> vertices4;
        vertices4.push_back(v2);
        vertices4.push_back(v0);
        vertices4.push_back(v3);

        objects.push_back(unique_ptr<Object>(new Triangle(vertices1, m3)));
        objects.push_back(unique_ptr<Object>(new Triangle(vertices2, m3)));
        objects.push_back(unique_ptr<Object>(new Triangle(vertices3, m3)));
        objects.push_back(unique_ptr<Object>(new Triangle(vertices4, m3)));
        objects.push_back(unique_ptr<Object>(new Sphere(center1, 0.25, m1)));
        objects.push_back(unique_ptr<Object>(new Sphere(center2, 0.5, m2)));
        objects.push_back(unique_ptr<Object>(new Plane(normal, p0, m4)));
        return objects;
    }

    vector<unique_ptr<Light>> buildLights()
    {
        Vector3f direction1(-1, -1, 0);
        Vector3f point1(4, 10, -8);
        vector<unique_ptr<Light>> lights;
        lights.push_back(unique_ptr<Light>(new AmbientLight()));
        lights.push_back(unique_ptr<Light>(new DistantLight(direction1)));
        lights.push_back(unique_ptr<Light>(new PointLight(point1)));

        return lights;
    }

public:
    Scene(vector<unsigned char> background = {0, 0, 0})
    {
        this->lights = buildLights();
        this->objects = buildObjects();
        this->background = background;
    }

    void showScene(unsigned char *image, const int width, const int height, bool perspective, int time)
    {
        // this code was used to create the animation frames
        // float x = sin(2 * M_PI * (30 - time) / 12 / 24);
        // float z = cos(2 * M_PI * (30 - time) / 12 / 24);

        Vector3f from(0, 0, 0);
        Vector3f to(2, 1, -9);
        camera.updateCamera(from, to, perspective);
        float aspectRatio = width / height;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // x and y shift code comes from https://www.scratchapixel.com/index.html
                int idx = (i * width + j) * 3;
                float x = (2 * (j + 0.5) / (float)width - 1) * aspectRatio;
                float y = (2 * (i + 0.5) / (float)height - 1);
                Vector3f origin;
                Vector3f direction;
                camera.getOrigin(x, y, origin);
                camera.getDirection(x, y, direction);
                float t = 100000;

                getBackground(image, idx);
                for (auto object = objects.begin(); object != objects.end(); ++object)
                {
                    HitRecord hit = object->get()->intersect(origin, direction, 0);
                    if (hit.t < t)
                    {
                        t = hit.t;
                        vector<unsigned char> color = {0, 0, 0};
                        for (auto light = lights.begin(); light != lights.end(); ++light)
                        {
                            light->get()->illuminate(hit, object->get(), color);
                        }
                        displayScreen(image, idx, color);
                    }
                }
            }
        }
    }
};