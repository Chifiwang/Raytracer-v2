#include "raytracer.hpp"

int main(void) {
  Camera cam;
  // ... cam settings
  cam.image_height = 400 * 9.0 / 16.0;
  cam.image_width = 400;

  // Object_list scene;
  // ... scene objects
  Sphere test(point3(0, 0, -1), 0.5);

  cam.render(test);
  return 0;
}
