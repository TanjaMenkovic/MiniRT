#include "../headers/minirt.h"

void    check_spheres(t_ray ray, t_rt rt, t_hit_record *h_rec)
{
    int     i;
    float   t;

    h_rec->t = -1;
    i = -1;
    while (++i < rt.num_sp)
    {
        t = hit_sphere(rt.sp[i].center, rt.sp[i].radius, ray);
        if (i == 0 || t > 0.0 && (h_rec->t < 0.0 || t < h_rec->t))
        {
            h_rec->t = t;
            h_rec->point = ray_point(ray, t);
            h_rec->normal = unit_vector(vec_sub(h_rec->point, rt.sp[i].center));
            h_rec->color = vec_mult(rt.sp[i].col, 255);
            h_rec->center = rt.sp[i].center;
            h_rec->id = i;
            h_rec->shape = 0;
        }
    }
}

void    check_planes(t_ray ray, t_rt rt, t_hit_record *h_rec)
{
    int     i;
    float   t;

    i = -1;
    while (++i < rt.num_pl)
    {
        t = hit_plane(rt.pl[i].normal, rt.pl[i].point, ray);
        if ((i == 0 && rt.num_sp == 0) || (t > 0.0 && (h_rec->t < 0.0 || t < h_rec->t)))
        {
            h_rec->t = t;
            h_rec->point = ray_point(ray, t);
            h_rec->normal = rt.pl[i].normal;
            h_rec->color = vec_mult(rt.pl[i].col, 255);
            h_rec->center = rt.pl[i].point;
            h_rec->id = i;
            h_rec->shape = 1;
        }
    }
}

void    check_cylinders(t_ray ray, t_rt rt, t_hit_record *h_rec)
{
    int     i;
    float   t;

    i = -1;
    while (++i < rt.num_cy)
    {
        t = hit_cylinder(rt.cy[i].center, rt.cy[i].normal, rt.cy[i].radius, rt.cy[i].height, ray);
        if ((i == 0 && (rt.num_sp == 0 && rt.num_pl == 0)) || (t > 0.0 && (h_rec->t < 0.0 || t < h_rec->t)))
        {
            h_rec->t = t;
            h_rec->point = ray_point(ray, t);
            h_rec->normal = cyl_normal(h_rec->point, rt.cy[i].center, rt.cy[i].normal, rt.cy[i].height, rt.cy[i].radius);
            h_rec->color = vec_mult(rt.cy[i].col, 255);
            h_rec->center = rt.cy[i].center;
            h_rec->id = i;
            h_rec->shape = 2;
        }
    }
}

/*
    Computes diffuse and specular lighting
    according to the Phong reflection model
    https://en.wikipedia.org/wiki/Phong_reflection_model
*/
void    compute_lighting(t_vector *color, t_rt rt, t_hit_record h_rec)
{
    float       diffuse_strength;
    float       specular_strength;
    t_vector    diffuse;
    t_vector    reflect_source;
    t_vector    specular;

    rt.l.light_color = (t_vector){1.0, 1.0, 1.0};
    rt.l.lighting = (t_vector){1.0, 1.0, 1.0};
    diffuse_strength = fmaxf(0.0, dot_prod(rt.l.light_dir, h_rec.normal));
    diffuse = vec_mult(rt.l.light_color, diffuse_strength);
    reflect_source = unit_vector(reflect(rt.l.point, h_rec.normal));
    specular_strength = fmaxf(0.0, dot_prod(rt.c.or_vec, reflect_source));
    specular_strength = powf(specular_strength, 32.0);
    specular = vec_mult(rt.l.light_color, specular_strength);
    rt.l.lighting = vec_add(vec_mult((t_vector){rt.a.ratio, rt.a.ratio, rt.a.ratio}, 0.8), vec_mult(diffuse, 0.5));
    rt.l.lighting = vec_add(vec_mult(specular, 0.0), rt.l.lighting);
    *color = (t_vector){h_rec.color.x * rt.l.lighting.x, h_rec.color.y * rt.l.lighting.y, h_rec.color.z * rt.l.lighting.z};
    color->x = fminf(fmaxf(color->x, 0.0), 255.0);
    color->y = fminf(fmaxf(color->y, 0.0), 255.0);
    color->z = fminf(fmaxf(color->z, 0.0), 255.0);
}

t_vector ray_color(t_ray ray, t_rt rt)
{
    t_hit_record h_rec;
    int     i;
    float   t;
    int     in_shadow;
    t_vector color;

    check_spheres(ray, rt, &h_rec);
    check_planes(ray, rt, &h_rec);
    check_cylinders(ray, rt, &h_rec);
    if (h_rec.t > 0.0)
    {
        rt.l.light_dir = unit_vector(vec_sub(rt.l.point, h_rec.point));
        check_shadow(rt, &in_shadow, h_rec);
        compute_lighting(&color, rt, h_rec);
         if (in_shadow == 1)
            color = vec_mult(color, 0.4);
        return (color);
    }
    return ((t_vector){255, 255, 255});
}
