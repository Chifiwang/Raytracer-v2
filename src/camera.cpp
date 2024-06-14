#include "camera.hpp"
#include "material.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include "object.hpp"

void Camera::init(Object *scene)
{
    m_scene = scene;

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

void Camera::render(Object* scene, std::ostream& output)
{
    init(scene);

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

color Camera::bounce_ray(const ray& r, int iter)
{
    if (iter < 0) {
        return color(0);
    }

    collision_history data;
    if (!m_scene->hit(r, data)) {
        return sky_box->sample(r.direction());
    }

    // vec3 N = ray_data.normal;
    // return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);

    // simulate light pdf
    vec3 point_light = vec3(2, 2, 1) + random_unit_vec3();
    vec3 l = normalize(point_light - data.collision);

    // shadow ray cast
    ray shadow = ray(data.collision, normalize(l - data.collision));
    collision_history shadow_data;

    color attenuation;
    if (m_scene->hit(shadow, shadow_data)) {
        return color(0);
    } else {
        // minimize expensive call to brdf
        attenuation = disney_brdf(l, r.direction(), data);
    }

    // compute rendering equation (assuming emitted light = 0)
    vec3 scatter_dir = random_unit_vec3_on_hemisphere(data.normal);
    ray scatter_ray = ray(data.collision, scatter_dir);

    return bounce_ray(scatter_ray, iter - 1) * attenuation;
}

color Camera::cast_ray(int r, int c)
{
    point3 pixel = m_px_00 + r * m_px_height + c * m_px_width;
    vec3 axis_dir = normalize(pixel - cam_pos);

    ray axis(cam_pos, axis_dir);

    // TODO: add depth of field & bouncing rays
    // Add pdf infra

    color res = color(0, 0, 0);

    // run a jank monte-carlo on each pixel
    for (int i = 0; i < sample_rate; ++i) {
        // Cast inital ray with depth of field variance
        point3 focus = axis.at(focus_dist);
        vec3 antialias_variance = random_on_unit_disk();
        vec3 aperature_variance = aperture * random_on_unit_disk();

        point3 light_origin = cam_pos
            + aperature_variance.x() * normalize(m_px_width)
            + aperature_variance.y() * normalize(m_px_height);

        vec3 light_dir = normalize(focus - light_origin);

        light_origin += .5 * antialias_variance.x() * m_px_width
            + .5 * antialias_variance.y() * m_px_height;

        ray cast_ray = ray(light_origin, light_dir);

        // start calculating bounces

        res += bounce_ray(cast_ray, simulation_depth);
    }
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

    output << int(255 * r) << ' ' << int(255 * g) << ' '
           << int(255 * b) << '\n';
}
