#include "../headers/ray.h"

t_ray   init_ray(t_vector s, t_vector d)
{
    t_ray   r;

    r.start = s;
    r.direction = d;
    r.t = RAY_MAX;
    return (r);
}

float   ray_point(t_ray r)
{
    return (r.start + r.t * r.direction);
}