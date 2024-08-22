#include "../headers/minirt.h"

float hit_sphere(t_vector center, float radius, t_ray ray)
{
    t_vector oc = vec_sub(center, ray.start);
    float a = dot_prod(ray.direction, ray.direction);
    float b = -2.0 * dot_prod(ray.direction, oc);
    float c = dot_prod(oc, oc) - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0)
        return (-1.0);
    return ((-b - sqrtf(discriminant)) / (2.0 * a));
}