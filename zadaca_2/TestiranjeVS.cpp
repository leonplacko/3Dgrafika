#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"

// struktura
typedef std::vector<Vec3f> Image;

using namespace std;

struct Light
{
    Vec3f pos;
    float intenstity;

    Light(const Vec3f& pos, const float& i) : pos(pos), intenstity(i) {}
};
typedef std::vector<Light> Lights;

struct Material
{
    Vec3f diffuse_color;
    Vec2f albedo;
    float specular_coef;


    Material(const Vec2f& albedo, const Vec3f& color, float exp) : diffuse_color(color), albedo(albedo), specular_coef(exp) {}
    Material() : albedo(Vec2f(1, 0)), diffuse_color(Vec3f(0, 0, 0)), specular_coef(1.f) {}
};

struct Object
{
    Material material;

    virtual bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const = 0;
    virtual Vec3f normal(const Vec3f& p) const = 0;
};
typedef std::vector<Object*> Objects;


// ovo dolje je cista kurcina
struct Cube : Object {

    Vec3f vmin;
    Vec3f vmax;

    Cube(const Vec3f& vmin1, const Vec3f vmax1, Material m) {

        vmax = vmax1;
        vmin = vmin1;
        Object::material = m;

    }


    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const{

        float txmin = (vmin[0] - p[0]) / d[0];
        float txmax = (vmax[0] - p[0]) / d[0];

        float tymin = ((vmin[1] - p[1]) / d[1]);
        float tymax = ((vmax[1] - p[1]) / d[1]);

        float tzmin = ((vmin[2] - p[2]) / d[2]);
        float tzmax = ((vmax[2] - p[2]) / d[2]);

        float txENT = min(txmin, txmax);
        float txEXT = max(txmin, txmax);
        float tyENT = min(tymin, tymax);
        float tyEXT = max(tymin, tymax);
        float tzENT = min(tzmin, tzmax);
        float tzEXT = max(tzmin, tzmax);

        if (txENT > tyEXT || txEXT < tyENT) {
            //cout << "1" << endl;
            return false;
        }
        else if (txENT > tzEXT || txEXT < tzENT) {
            //cout << "2" << endl;
            return false;
        }
        else {
            float mini = max(txENT, tyENT);
            float maxi = min(txEXT, tyEXT);
            t = max(mini, tzENT);
            if (t < 0) {
                t = min(maxi, tzEXT);
            }
            if (t < 0) {
                return false;
            }
            else {
                //cout << "3" << endl;
                return true;
            }
        }

    }

    Vec3f normal(const Vec3f& p) const {
        return (p - vmin).normalize();
    }

};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius

    Sphere(const Vec3f& c, const float& r, const Material& mat) : c(c), r(r)
    {
        Object::material = mat;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const
    {
        Vec3f v = c - p; // vektor izmedju izvora zrake i centra

        if (d * v < 0) // skalarni produkt
        {
            // sfera se nalazi iza zrake
            return false;
        }
        else
        {
            // izracunaj projekciju
            Vec3f pc = p + d * ((d * v) / d.norm());
            if ((c - pc) * (c - pc) > r * r)
            {
                // nema sjeciste
                return false;
            }
            else
            {
                float dist = sqrt(r * r - (c - pc) * (c - pc));

                if (v * v > r * r) // izvor izvan sfere
                {
                    t = (pc - p).norm() - dist;
                }
                else // izvor unutar sfere
                {
                    t = (pc - p).norm() + dist;
                }

                return true;
            }
        }
    }

    Vec3f normal(const Vec3f& p) const
    {
        return (p - c).normalize();
    }
};

struct Cylinder : Object {
    
    Vec3f center;
    float radius;
    float height;

    Cylinder(Vec3f c, float r, float h, Material m) {
        center = c;
        radius = r;
        height = h;
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const {
        
        Vec3f v = p - center;
            
        float a = (d.x * d.x) + (d.z * d.z);
        float b = 2 * (d.x * (p.x - center.x) + d.z * (p.z - center.z));
        float c = (p.x - center.x) * (p.x - center.x) + (p.z - center.z) * (p.z - center.z) - (radius * radius);

        float delta = b * b - 4 * (a * c);

        if (v * d < 0) {
            //cout << "1" << endl;
            return false;
        }
        else {

            if (delta < 0.0) {
                //cout << "2" << endl;
                return false;
            }

            float t1 = min((-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));
            float t2 = max((-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));


            float nek = p[1] + t1 * d[1];
            float nek2 = p[1] + t2 * d[1];


            if (nek >= center[1] && nek <= center[1] + height) {
                t = t1;
                return true;
            }
            else if (nek2 >= center[1] && nek2 <= center[1] + height) {
                t = t2;
                return true;
            }
            else {
                //cout << "3" << endl;
                return false;
            }
        }

    }

    Vec3f normal(const Vec3f& p) const {
        Vec3f n = (p - center).normalize();
        n.y = 0;
        //n = -n;
        return n;
    }


};


// funkcija koja koristimo za crtanje scene
bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const Objects& objs, Vec3f& hit_point, Material& hit_material, Vec3f& hit_normal)
{
    float dist = std::numeric_limits<float>::max();
    float obj_dist = std::numeric_limits<float>::max();

    for (auto obj : objs)
    {
        if (obj->ray_intersect(orig, dir, obj_dist) && obj_dist < dist)
        {
            dist = obj_dist;                     // udaljenost do sfere
            hit_point = dir * obj_dist;          // tocka sjecista
            hit_material = obj->material;        // materijal sfere
            hit_normal = obj->normal(hit_point); // normala na sjeciste
        }
    }

    return dist < 1000;
}

Vec3f reflex(Vec3f d, Vec3f n) {
    return d - ((n *(d * n)) * 2);
}

Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const Objects& objs, const Lights& lights, int dubina)
{
    Vec3f hit_point;
    Vec3f hit_normal;
    Material hit_material;

    if (!scene_intersect(orig, dir, objs, hit_point, hit_material, hit_normal))
    {
        return Vec3f(1, 0.7, 0.4); // vrati boju pozadine
    }
    else
    {
        float diffuse_color_intensity = 0;
        float specular_light_intensity = 0;


        for (auto light : lights)
        {
            Vec3f light_direction = (light.pos - hit_point).normalize();

            Vec3f shadow_origin;
            Vec3f shadow_hit_normal;
            Vec3f shadow_hitpoint;
            Material shadow_material;

            if (hit_normal * light_direction < 0) {
                hit_normal = -hit_normal;
            }

            if (light_direction * hit_normal < 0) {
                shadow_origin = hit_point - hit_normal * 0.001;
            }
            else {
                shadow_origin = hit_point + hit_normal * 0.001;
            }

            float light_dist = (light.pos - hit_point).norm();
            if (scene_intersect(shadow_origin, light_direction, objs, shadow_hitpoint, shadow_material, shadow_hit_normal) && (shadow_hitpoint - shadow_origin).norm() < light_dist) {
                continue;
            }

            // lambertov model
            diffuse_color_intensity += light.intenstity * max(0.f, light_direction * hit_normal);
            // bling phong model
            Vec3f v = (orig - hit_point).normalize();
            Vec3f h = (v + light_direction).normalize();

            specular_light_intensity += light.intenstity * powf(max(0.f, hit_normal * h), hit_material.specular_coef);
        }

        Vec3f R = reflex(dir, hit_normal);

        Vec3f c = hit_material.diffuse_color * hit_material.albedo[0] * diffuse_color_intensity + hit_material.diffuse_color * hit_material.albedo[1] * specular_light_intensity;

        if (dubina != 1) {
            return c + cast_ray(orig, R, objs, lights, dubina - 1) * 0.1;
        }
        else if (dubina == 1) {
            return c;
        }

    }
}

// funkcija za crtanje
void render(const Objects& objects, const Lights& lights)
{
    // velicina slike
    const int width = 1024;
    const int height = 768;
    const int fov = 3.1415926 / 2.0;

    // spremnik za sliku
    Image buffer(width * height);

    // nacrtaj u sliku
    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            // pošalji zraku u svaki piksel
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);

            // definiraj smjer
            Vec3f dir = Vec3f(x, y, -1).normalize();

            buffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, objects, lights, 3);
        }
    }

    // spremanje slike u vanjsku datoteku
    std::ofstream ofs;
    ofs.open("./sfere.ppm", ofstream::binary);
    // oblikuj po ppm formatu
    ofs << "P6\n"
        << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            ofs << (unsigned char)(255.f * std::max(0.f, std::min(1.f, buffer[i][j])));
        }
    }
    // zatvori datoteku
    ofs.close();
}

int main()
{
    Material red(Vec2f(0.6, 0.4), Vec3f(1, 0, 0), 60);
    Material green(Vec2f(0.5, 0.5), Vec3f(0, 0.5, 0), 60);
    Material blue(Vec2f(0.7, 0.3), Vec3f(0, 0, 1), 10);
    Material grey(Vec2f(0.8, 0.2), Vec3f(0.5, 0.5, 0.5), 10);
    Material purple(Vec2f(0.4, 0.6), Vec3f(0.8, 0, 1), 30);

    // definiraj objekte u sceni
    Sphere s1(Vec3f(-3, 0, -16), 2, red);
    Sphere s2(Vec3f(-1.0, -1.5, -12), 2, purple);
    Sphere s3(Vec3f(1.5, -0.5, -18), 3, blue);
    Cube s4(Vec3f(2, 2, -20), Vec3f(5, 5, -25), blue);
    Cylinder cl1(Vec3f(15, 5, 30), 2, 8, purple);

    // definiranje svjetla
    Light l1 = Light(Vec3f(-20, 20, 20), 1.5);
    Light l2 = Light(Vec3f(20, 30, 20), 1.8);

    Lights lights = { l1, l2 };
    Objects objs = { &s2, &cl1, &s4, &s1, &s3 };

    render(objs, lights);

    cout << "Dovršeno" << endl;
    return 0;
}