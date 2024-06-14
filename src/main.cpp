#include "raytracer.hpp"
#include <memory>

int main(void) {
  Camera cam;
  // ... cam settings
  cam.image_height = 400 * 9.0 / 16.0;
  cam.image_width = 400;
  cam.focus_dist = 1;
  cam.aperture = 0;

  cam.simulation_depth = 8;
  cam.sample_rate = 16;

  Gradient sky_box = Gradient(color(1), color(0.5, 0.7, 1));
  cam.sky_box = &sky_box;

  // Object_list scene;
  // ... scene objects
  Material sphere_mat;
  Solid sphere_text(color(1, 0, 0));
  sphere_mat.base_texture = &sphere_text;
  Sphere test(point3(0, 0, -1), 0.5, &sphere_mat);
  Sphere test2(point3(0, -100.5, -1), 100, &sphere_mat);

  Collection world;
  world.add(&test);
  world.add(&test2);

  cam.render(&world);
  return 0;
}
