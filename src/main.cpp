#include "raytracer.hpp"
#include <memory>

int main(void) {
  Camera cam;
  // ... cam settings
  cam.image_height = 400 * 9.0 / 16.0;
  cam.image_width = 400;
  cam.focus_dist = 1;
  cam.aperture = .02;

  cam.sky_box = std::make_shared<Gradient>(color(1), color(0.5, 0.7, 1));

  // Object_list scene;
  // ... scene objects
  auto sphere_mat = std::make_shared<Material>();
  sphere_mat->base_texture = std::make_shared<Solid>(color(1, 0, 0));
  Sphere test(point3(0, 0, -1), 0.5, sphere_mat);

  cam.render(test);
  return 0;
}
