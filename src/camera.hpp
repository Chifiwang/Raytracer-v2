#ifndef CAMERA_HPP
#define CAMERA_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"
// #include "matrix.hpp"

using angle = double;

// struct Element {
//     unsigned object_id;
//     point3 position;
//
//     void rotate(double x, double y, double z);
//     void scale(double s);
// private:
//     mat3x3 transform;
//     mat3x3 inv_transform;
// };
//
// struct Scene {
//     std::vector<Element> elements;
//     std::vector<Object*> objects;
// };

class Camera {
    double m_aspect_ratio;
    double m_sampling_weight;

    vec3 m_px_height;
    vec3 m_px_width;
    vec3 m_px_00;

    const bvh* m_scene = nullptr;
    const Object* m_objects;
    const Object* m_lights;

    void init(Object* scene, Object* lights);
    void write_pixel(color c, std::ostream& output);
    color bounce_ray(const ray& r);
    color cast_ray(int r, int c);
    color color_ray(const ray& r);

public:
    int image_height = 100;
    int image_width = 100;
    int sample_rate = 10;
    int simulation_depth = 10; // bounce iterations

    angle fov = 90.0;
    double focal_length = 1.0;
    double aperture = 0.0;

    point3 cam_pos = point3(0);
    vec3 cam_tilt = vec3(0, 1, 0); // points up
    point3 subject_pos = point3(0, 0, -1);
    double focus_dist = 1; // distance from camera that you want to be in focus

    Texture* sky_box = nullptr;

    Camera() { }
    ~Camera() { delete m_scene; }
    void render(Object* scene, Object* lights, std::ostream& output = std::cout);
};

#endif // !CAMERA_HPP
