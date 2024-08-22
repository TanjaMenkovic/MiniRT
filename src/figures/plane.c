#include "../headers/minirt.h"

float hit_plane(t_vector normal, t_vector point, t_ray ray)
{
    float denom;

    denom = dot_prod(normal, ray.direction);
    if (fabs(denom) > 0.0001f)
    {
        return (dot_prod(vec_sub(point, ray.start), normal) / denom);
    }
    return (-1.0);
}