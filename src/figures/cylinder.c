#include "../headers/minirt.h"

float hit_top_cap(t_vector axis_dir, t_vector top_cap_center, t_ray ray, float radius)
{
    float   t_cap_top;
    t_vector point_top;
    t_vector vec_top;

    t_cap_top = hit_plane(axis_dir, top_cap_center, ray);
    if (t_cap_top > 0.0f) {
        point_top = vec_add(ray.start, vec_mult(ray.direction, t_cap_top));
        vec_top = vec_sub(point_top, top_cap_center);
        if (dot_prod(vec_top, vec_top) > radius * radius)
            t_cap_top = -1.0f;
    }
    return (t_cap_top);
}

float   hit_bot_cap(t_vector axis_dir, t_vector base_center, t_ray ray, float radius)
{
    float t_cap_bottom;
    t_vector point_bottom;
    t_vector vec_bottom;

    t_cap_bottom = hit_plane(axis_dir, base_center, ray);
    if (t_cap_bottom > 0.0f) {
        point_bottom = vec_add(ray.start, vec_mult(ray.direction, t_cap_bottom));
        vec_bottom = vec_sub(point_bottom, base_center);
     if (dot_prod(vec_bottom, vec_bottom) > radius * radius)
        t_cap_bottom = -1.0f;
    }
    return (t_cap_bottom);
}

float hit_cyl_body(t_ray ray, t_vector base_center, t_vector axis_dir, float radius, float height)
{
    t_cyl_body cy;

    cy.oc = vec_sub(ray.start, base_center);
    cy.oc_perp = vec_sub(cy.oc, vec_mult(axis_dir, dot_prod(cy.oc, axis_dir)));
    cy.ray_dir_perp = vec_sub(ray.direction, vec_mult(axis_dir, dot_prod(ray.direction, axis_dir)));
    cy.a = dot_prod(cy.ray_dir_perp, cy.ray_dir_perp);
    cy.b = 2.0f * dot_prod(cy.ray_dir_perp, cy.oc_perp);
    cy.c = dot_prod(cy.oc_perp, cy.oc_perp) - radius * radius;
    cy.discriminant = cy.b * cy.b - 4.0f * cy.a * cy.c;
    if (cy.discriminant < 0.0f)
        return -1.0f;
    cy.sqrt_discriminant = sqrtf(cy.discriminant);
    cy.t1 = (-cy.b - cy.sqrt_discriminant) / (2.0f * cy.a);
    cy.t2 = (-cy.b + cy.sqrt_discriminant) / (2.0f * cy.a);
    cy.t_cylinder = -1.0f;
    cy.p1 = vec_add(ray.start, vec_mult(ray.direction, cy.t1));
    cy.p2 = vec_add(ray.start, vec_mult(ray.direction, cy.t2));
    cy.z1 = dot_prod(vec_sub(cy.p1, base_center), axis_dir);
    cy.z2 = dot_prod(vec_sub(cy.p2, base_center), axis_dir);
    if (cy.t1 > 0 && cy.z1 >= 0.0f && cy.z1 <= height)
        cy.t_cylinder = cy.t1;
    else if (cy.t2 > 0 && cy.z2 >= 0.0f && cy.z2 <= height)
        cy.t_cylinder = cy.t2;
    return (cy.t_cylinder);
}

float hit_cylinder(t_vector base_center, t_vector axis_dir, float radius, float height, t_ray ray)
{
    float t_cylinder;
    float t_cap_top;
    float t_cap_bottom;
    t_vector top_cap_center;


    axis_dir = unit_vector(axis_dir);
    t_cylinder = hit_cyl_body(ray, base_center, axis_dir, radius, height);
    top_cap_center = vec_add(base_center, vec_mult(axis_dir, height));
    t_cap_top = hit_top_cap(axis_dir, top_cap_center, ray, radius);
    t_cap_bottom = hit_bot_cap(axis_dir, base_center, ray, radius);
    if (t_cylinder < 0.0f || (t_cap_top > 0.0f && t_cap_top < t_cylinder))
        t_cylinder = t_cap_top;
    if (t_cylinder < 0.0f || (t_cap_bottom > 0.0f && t_cap_bottom < t_cylinder))
        t_cylinder = t_cap_bottom;
    return t_cylinder;
}


t_vector cyl_normal(t_vector point, t_vector base_center, t_vector axis_dir, float height, float radius)
{
    float   proj_length;
    t_vector p_minus_c;
    t_vector proj_p_minus_c;
    t_vector normal;
    
    axis_dir = unit_vector(axis_dir);
    p_minus_c = vec_sub(point, base_center);
    proj_length = dot_prod(p_minus_c, axis_dir);
    if (proj_length >= height)
        return axis_dir;
    else if (proj_length <= 0)
        return vec_mult(axis_dir, -1.0);
    proj_p_minus_c = vec_mult(axis_dir, proj_length);
    normal = vec_sub(p_minus_c, proj_p_minus_c);
    normal = unit_vector(normal);
    return normal;
}

