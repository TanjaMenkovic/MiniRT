#include "../headers/minirt.h"

float hit_cylinder(t_vector base_center, t_vector axis_dir, float radius, float height, t_ray ray) {
    // Normalize axis direction
    axis_dir = unit_vector(axis_dir);

    // Vector from base_center to ray start
    t_vector oc = vec_sub(ray.start, base_center);

    // Components perpendicular to the axis
    t_vector oc_perp = vec_sub(oc, vec_mult(axis_dir, dot_prod(oc, axis_dir)));
    t_vector ray_dir_perp = vec_sub(ray.direction, vec_mult(axis_dir, dot_prod(ray.direction, axis_dir)));

    // Quadratic coefficients for the infinite cylinder
    float a = dot_prod(ray_dir_perp, ray_dir_perp);
    float b = 2.0f * dot_prod(ray_dir_perp, oc_perp);
    float c = dot_prod(oc_perp, oc_perp) - radius * radius;

    // Discriminant
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return -1.0f;

    // Find the nearest valid intersection with the infinite cylinder
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    float t_cylinder = -1.0f;

    // Calculate the intersection points in 3D space
    t_vector p1 = vec_add(ray.start, vec_mult(ray.direction, t1));
    t_vector p2 = vec_add(ray.start, vec_mult(ray.direction, t2));

    // Check if the intersections are within the cylinder's height
    float z1 = dot_prod(vec_sub(p1, base_center), axis_dir);
    float z2 = dot_prod(vec_sub(p2, base_center), axis_dir);

    if (t1 > 0 && z1 >= 0.0f && z1 <= height)
        t_cylinder = t1;
    else if (t2 > 0 && z2 >= 0.0f && z2 <= height)
        t_cylinder = t2;

    // Intersection with the top cap plane
    t_vector top_cap_center = vec_add(base_center, vec_mult(axis_dir, height));
    float t_cap_top = hit_plane(axis_dir, top_cap_center, ray);

    // Check if the intersection point lies within the top cap's radius
    if (t_cap_top > 0.0f) {
        t_vector point_top = vec_add(ray.start, vec_mult(ray.direction, t_cap_top));
        t_vector vec_top = vec_sub(point_top, top_cap_center);
        if (dot_prod(vec_top, vec_top) > radius * radius)
            t_cap_top = -1.0f;
    }

    // Intersection with the bottom cap plane
    float t_cap_bottom = hit_plane(axis_dir, base_center, ray);

    // Check if the intersection point lies within the bottom cap's radius
    if (t_cap_bottom > 0.0f) {
        t_vector point_bottom = vec_add(ray.start, vec_mult(ray.direction, t_cap_bottom));
        t_vector vec_bottom = vec_sub(point_bottom, base_center);
        if (dot_prod(vec_bottom, vec_bottom) > radius * radius)
            t_cap_bottom = -1.0f;
    }

    // Return the closest valid intersection (if any)
    float t_final = t_cylinder;

    if (t_final < 0.0f || (t_cap_top > 0.0f && t_cap_top < t_final))
        t_final = t_cap_top;

    if (t_final < 0.0f || (t_cap_bottom > 0.0f && t_cap_bottom < t_final))
        t_final = t_cap_bottom;

    return t_final;
}


t_vector cyl_normal(t_vector point, t_vector base_center, t_vector axis_dir, float height, float radius) {
    // Normalize the cylinder's axis direction
    axis_dir = unit_vector(axis_dir);

    // Vector from base center to the intersection point
    t_vector p_minus_c = vec_sub(point, base_center);

    // Project this vector onto the cylinder's axis
    float proj_length = dot_prod(p_minus_c, axis_dir);

    // Check if the point is on the top or bottom cap
    if (proj_length >= height) {
        // Point is on the top cap
        return axis_dir;  // Normal is the axis direction
    } else if (proj_length <= 0) {
        // Point is on the bottom cap
        return vec_mult(axis_dir, -1.0);  // Normal is the opposite of the axis direction
    }

    // If the point is on the body, subtract the projection from p_minus_c to get the perpendicular component
    t_vector proj_p_minus_c = vec_mult(axis_dir, proj_length);
    t_vector normal = vec_sub(p_minus_c, proj_p_minus_c);

    // Normalize the resulting vector to get the normal
    normal = unit_vector(normal);

    return normal;
}

