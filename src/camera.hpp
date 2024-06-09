#ifndef CAMERA_HPP
#define CAMERA_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <iostream>

using angle = double;

class Camera {
  double m_aspect_ratio;
  double m_sampling_weight;

  vec3 m_px_height;
  vec3 m_px_width;
  vec3 m_px_00;

  void init();
  void write_pixel(color c, std::ostream &output);
  color cast_ray(Object &scene, int r, int c);
  color color_ray(const ray &r);

public:
  int image_height = 100;
  int image_width = 100;
  int sample_rate = 10;
  int simulation_depth = 10; // bounce iterations

  angle fov = 90.0;
  double focal_length = 1.0;
  double aperture = 0.0;

  point3 cam_pos = point3(0, 0, 0);
  vec3 cam_tilt = vec3(0, 1, 0); // points up
  point3 subject_pos = point3(0, 0, -1);

  Camera() {}
  void render(Object &scene, std::ostream &output = std::cout);
};

#endif // !CAMERA_HPP
