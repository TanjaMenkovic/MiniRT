#include "../headers/minirt.h"

// Intersecting with the cone body
static float hit_cone_body(t_ray ray, t_vector apex, t_vector axis_dir, float radius, float height, float angle)
{
    float t_cone;
    t_cone_body cone;
    float tan_theta2 = tanf(angle) * tanf(angle);
    t_vector oc;
    t_vector ray_dir;
    t_vector oc_perp;
    t_vector ray_dir_perp;
    float a, b, c, discriminant;

    oc = vec_sub(ray.start, apex);
    ray_dir = ray.direction;
    
    // Project ray direction and oc perpendicular to the axis direction
    oc_perp = vec_sub(oc, vec_mult(axis_dir, dot_prod(oc, axis_dir)));
    ray_dir_perp = vec_sub(ray_dir, vec_mult(axis_dir, dot_prod(ray_dir, axis_dir)));

    a = dot_prod(ray_dir_perp, ray_dir_perp) - tan_theta2 * dot_prod(ray_dir, axis_dir) * dot_prod(ray_dir, axis_dir);
    b = 2.0f * (dot_prod(ray_dir_perp, oc_perp) - tan_theta2 * dot_prod(ray_dir, axis_dir) * dot_prod(oc, axis_dir));
    c = dot_prod(oc_perp, oc_perp) - tan_theta2 * dot_prod(oc, axis_dir) * dot_prod(oc, axis_dir);
    
    discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return -1.0f;
    
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    t_cone = -1.0f;
    t_cone = (t1 > 0 && t1 < t2) ? t1 : t2;
    
    // Check height bounds
    t_vector point = vec_add(ray.start, vec_mult(ray.direction, t_cone));
    float z = dot_prod(vec_sub(point, apex), axis_dir);
    
    if (z < 0 || z > height)
        return -1.0f;
    
    return t_cone;
}

// Calculate the normal at a point on the cone
t_vector cone_normal(t_vector point, t_vector apex, t_vector axis_dir, float radius, float height, float angle)
{
    t_vector p_minus_apex;
    t_vector normal;
    float tan_theta2 = tanf(angle) * tanf(angle);
    float proj_length;

    p_minus_apex = vec_sub(point, apex);
    proj_length = dot_prod(p_minus_apex, axis_dir);
    
    if (proj_length <= 0)
        return axis_dir; // Normal at the base
    
    if (proj_length >= height)
        return vec_mult(axis_dir, -1.0f); // Normal at the apex

    t_vector p_on_cone = vec_add(apex, vec_mult(axis_dir, proj_length));
    t_vector p_on_cone_perp = vec_sub(point, p_on_cone);
    
    // Normalize the normal vector
    normal = vec_mult(axis_dir, tan_theta2);
    normal = vec_sub(p_on_cone_perp, normal);
    normal = unit_vector(normal);
    
    return normal;
}

// Intersection function for cone
float hit_cone(t_vector apex, t_vector axis_dir, float radius, float height, float angle, t_ray ray)
{
    float t_cone;
    
    axis_dir = unit_vector(axis_dir);

    t_cone = hit_cone_body(ray, apex, axis_dir, radius, height, angle);
    
    return t_cone;
}
