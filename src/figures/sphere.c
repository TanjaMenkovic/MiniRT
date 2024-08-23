#include "../headers/minirt.h"

float hit_sphere(t_vector center, float radius, t_ray ray)
{
    t_vector    oc;
    float       a;
    float       b;
    float       c;
    float       discriminant;   
    
    oc = vec_sub(center, ray.start);
    a = dot_prod(ray.direction, ray.direction);
    b = -2.0 * dot_prod(ray.direction, oc);
    c = dot_prod(oc, oc) - radius * radius;
    discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return (-1.0);
    return ((-b - sqrtf(discriminant)) / (2.0 * a));
}
