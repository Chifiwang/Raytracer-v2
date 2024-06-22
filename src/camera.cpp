#include "camera.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "object.hpp"
#include "utils.hpp"
#include "vec3.hpp"

void Camera::init(Object* scene, Object* lights)
{
    m_scene = new bvh(scene);
    m_lights = lights;

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

    m_sqrt_sample_rate = std::sqrt(sample_rate);
    m_sqrt_inv_sample_rate = 1.0 / m_sqrt_sample_rate;
    m_sampling_weight = sq(m_sqrt_inv_sample_rate);
}

void Camera::render(Object* scene, Object* lights, std::ostream& output)
{
    init(scene, lights);

    // Write ppm header
    output << "P3\n"
           << image_width << ' ' << image_height << "\n255\n";

    for (int r = 0; r < image_height; ++r) {
        std::clog << '\r' << 100 * r / (image_height - 1) << "% Completed"
                  << std::flush;

        for (int c = 0; c < image_width; ++c) {
            color px_color = cast_ray(r, c);
            write_pixel(px_color, output);
        }
    }

    std::clog << "\rRender Completed                                   \n";
}

color Camera::bounce_ray(const ray& r)
{
    ray curr = r;
    color light { 0 };
    color contribution { 1 };

    collision_history data;
    // NOTE: Each iteration is one recursive iteration of the recursive rendering equation
    for (int i = 0; i < simulation_depth; ++i) {

        data = collision_history();
        if (!m_scene->hit(curr, data)) {
            light += sky_box->sample(curr.direction) * contribution;
            break;
        }

        // NOTE: Models the Light emitted part of the lighting equation
        light += data.mat->emission_color * data.mat->emission_power;

        // NOTE: Calculate where the incident light came from and repeat rendering eq on that
        vec3 scatter_dir = scatter(r.direction, data);

        // NOTE: Models the absorption of certain light frequencies and their intensity
        contribution *= brdf(scatter_dir, curr.direction, data)
            * data.tex->sample(data.collision)
            * dot(data.normal, scatter_dir);

        // prepare for next iteration;
        curr = ray(data.collision, scatter_dir);
    }
    collision_history shadow_data;
    point3 light_pos = m_lights->random_point();
    ray shadow_ray = ray(light_pos, light_pos - data.collision);
    if (m_scene->hit(shadow_ray, shadow_data)) {
        return color { 0 };
    }
    return light;
}

color Camera::cast_ray(int r, int c)
{
    // TODO: add depth of field & bouncing rays
    // Add pdf infra

    color res = color(0);

    // run a jank monte-carlo on each pixel for the path tracer
    for (int i = 0; i < m_sqrt_sample_rate; ++i) {
        for (int j = 0; j < m_sqrt_sample_rate; ++j) {
            // Cast inital ray with depth of field variance
            vec3 antialias_variance_x = (i + random_double()) * m_sqrt_inv_sample_rate - .5;
            vec3 antialias_variance_y = (j + random_double()) * m_sqrt_inv_sample_rate - .5;
            vec3 aperature_variance = aperture * random_on_unit_disk();

            point3 pixel = m_px_00 + (r + antialias_variance_y) * m_px_height
                + (c + antialias_variance_x) * m_px_width;
            vec3 axis_dir = normalize(pixel - cam_pos);

            ray axis(cam_pos, axis_dir);
            point3 focus = axis.at(focus_dist);
            // hard coded light for now
            point3 light_origin = cam_pos
                + aperature_variance.x * normalize(m_px_width)
                + aperature_variance.y * normalize(m_px_height);

            vec3 light_dir = normalize(focus - light_origin);

            ray cast_ray = ray(light_origin, light_dir);

            // start calculating bounces

            res += bounce_ray(cast_ray);
        }
    }
    return res / sample_rate;
}

void Camera::write_pixel(color c, std::ostream& output)
{
    double r = c.x;
    double g = c.y;
    double b = c.z;

    if (r != r)
        r = 0.0;
    if (g != g)
        g = 0.0;
    if (b != b)
        b = 0.0;

    r = Interval::Unit.clamp(r);
    g = Interval::Unit.clamp(g);
    b = Interval::Unit.clamp(b);

    output << int(255 * r) << ' ' << int(255 * g) << ' '
           << int(255 * b) << '\n';
}
