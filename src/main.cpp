#include "raytracer.hpp"
#include "texture.hpp"

int main(void)
{
    Camera cam;
    // ... cam settings
    cam.image_height = 400 * 9.0 / 16.0;
    cam.image_width = 400;
    cam.focus_dist = 1;
    // cam.focal_length = 100;
    // cam.fov = 1;
    cam.aperture = 0;

    cam.simulation_depth = 4;
    cam.sample_rate = 1000;

    // Gradient sky_box = Gradient(color(1), color(0.5, 0.7, 1));
    Solid sky_box = Solid(0);
    cam.sky_box = &sky_box;

    cam.cam_pos = point3(-7, 2, -2);
    cam.subject_pos = point3(0);
    cam.fov = 20;

    // Object_list scene;
    // ... scene objects
    Material sphere_mat;
    sphere_mat.specular = 1;
    sphere_mat.roughness = .75;
    Solid sphere_text(color(1, 0, 1));
    Solid sphere_text2(color(.2, .3, 1));
    Solid sphere_text3(color(.8, .5, .2));

    Material sphere_mat3;

    Material sphere_mat2;
    sphere_mat2.emission_color = color(.8, .5, .2);
    sphere_mat2.emission_power = 10;
    Sphere test(point3(0, 0, -1), 0.5, &sphere_mat, &sphere_text);
    Sphere test2(point3(0, -100.5, -1), 100, &sphere_mat3, &sphere_text2);
    Sphere test3(point3(15, 3, -12), 11, &sphere_mat2, &sphere_text3);

    Collection world;
    world.add(&test);
    world.add(&test2);
    world.add(&test3);

    cam.render(&world, &test3);
    return 0;
}
