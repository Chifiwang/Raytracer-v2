#include "camera.hpp"
#include "material.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include <iostream>

void Camera::init()
{
    m_aspect_ratio = double(image_width) / image_height;
    double frame_h_len = 2 * tan(to_radians(fov) / 2) * focal_length;
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

    // TODO: add depth of field & bouncing rays
    // Add pdf infra

    color res = color(0, 0, 0);
    for (int i = 0; i < sample_rate; ++i) {
        point3 focus = axis.at(focus_dist); // should be at cam_pos + axis_dir
        vec3 antialias_variance = random_on_unit_disk(); // antialias variance param
        vec3 aperature_variance = aperture * random_on_unit_disk();

        point3 light_origin = cam_pos
            + aperature_variance.x() * normalize(m_px_width)
            + aperature_variance.y() * normalize(m_px_height);

        vec3 light_dir = normalize(focus - light_origin);

        light_origin += antialias_variance.x() * m_px_width
            + antialias_variance.y() * m_px_height;

        ray cast_ray = ray(light_origin, light_dir);

        // std::cerr << cast_ray.direction() - axis.direction() << '\n';
        // std::cerr << cast_ray.origin() - axis.origin() << '\n';

        collision_history ray_data;
        ray_data.min_dist = Interval(0.001, infty);
        // //
        if (!scene.hit(cast_ray, ray_data)) {
            // vec3 N = ray_data.normal;
            // return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
            res += sky_box->sample(axis.direction());
            continue;
        }

        // vec3 N = ray_data.normal;
        // return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
        vec3 point_light = vec3(2, 2, 1);
        vec3 l = point_light - ray_data.collision;
        res += disney_brdf(l, cast_ray.direction(), ray_data);

        // net color
        // for (int i = 0; i < simulation_depth; ++i) {
        //     scatter ray
        //     if light compute color
        //     else scatter again
        //     accumulate color from "recursive" computation
    }
    // return normalized net color
    return res / sample_rate;
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
