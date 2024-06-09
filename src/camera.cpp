#include "camera.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include <iostream>

void Camera::init()
{
    m_aspect_ratio = double(image_width) / image_height;
    double frame_h_len = 2 * tan(to_radians(fov) / 2);
    double frame_w_len = frame_h_len * m_aspect_ratio;

    vec3 cam_dir = normalize(cam_pos - subject_pos);
    vec3 frame_w_dir = normalize(cross(cam_tilt, cam_dir));
    vec3 frame_h_dir = cross(frame_w_dir, cam_dir);

    vec3 frame_w = frame_w_len * frame_w_dir;
    vec3 frame_h = frame_h_len * frame_h_dir;

    m_px_width = frame_w / image_width;
    m_px_height = frame_h / image_height;

    m_px_00 = cam_pos - cam_dir - (frame_h + frame_w - m_px_height - m_px_width) / 2;

    m_sampling_weight = 1.0 / sample_rate;
}

void Camera::render(Object& scene, std::ostream& output)
{
    init();

    // Write ppm header
    output << "P3\n"
           << image_width << ' ' << image_height << "\n255\n";

    for (int r = 0; r < image_height; ++r) {
        std::clog << '\r' << 100 * r / (image_height - 1) << "% Completed"
                  << std::flush;

        for (int c = 0; c < image_width; ++c) {
            color px_color = cast_ray(scene, r, c);
            write_pixel(px_color, output);
        }
    }

    std::clog << "\rRender Completed                                   \n";
}

color Camera::cast_ray(Object& scene, int r, int c)
{
    point3 pixel = m_px_00 + r * m_px_height + c * m_px_width;
    vec3 axis_dir = normalize(pixel - cam_pos);

    ray axis(cam_pos, axis_dir);

    // TODO: add depth of field

    collision_history ray_data;
    ray_data.min_dist = Interval(0.001, infty);

    if (scene.hit(axis, ray_data)) {
        vec3 N = ray_data.normal;
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    return color_ray(axis);
}

color Camera::color_ray(const ray& r)
{
    double a = 0.5 * (r.direction().y() + 1);
    return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
}

void Camera::write_pixel(color c, std::ostream& output)
{
    double r = c.x();
    double g = c.y();
    double b = c.z();

    if (r != r)
        r = 0.0;
    if (g != g)
        g = 0.0;
    if (b != b)
        b = 0.0;

    output << int(255.999 * r) << ' ' << int(255.999 * g) << ' '
           << int(255.999 * b) << '\n';
}
