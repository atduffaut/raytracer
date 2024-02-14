#include <vector>

using namespace std;

struct Material
{
    // Idea for Material struct comes from "Fundamentals of Computer Graphics" 5th Edition
    vector<unsigned char> ca;
    vector<unsigned char> cd;
    vector<unsigned char> cs;
    float ka;
    float kd;
    float ks;
    float km;
    Material(vector<unsigned char> ca, vector<unsigned char> cd, vector<unsigned char> cs, float ka, float kd, float ks, float km)
    {
        this->ca = ca;
        this->cd = cd;
        this->cs = cs;
        this->ka = ka;
        this->kd = kd;
        this->ks = ks;
        this->km = km;
    }
    Material() {}
};