#include "../headers/minirt.h"

t_vector ray_color(t_ray ray, t_rt rt)
{
    t_vector N;
    t_hit_record h_rec;
    float   t;
    int     i;

    h_rec.t = -1;
    i = -1;
    while (++i < rt.num_sp)
    {
        t = hit_sphere(rt.sp[i].center, rt.sp[i].radius, ray);
        if (i == 0 || t > 0.0 && (h_rec.t < 0.0 || t < h_rec.t))
        {
            h_rec.t = t;
            h_rec.point = ray_point(ray, t);
            h_rec.normal = unit_vector(vec_sub(h_rec.point, rt.sp[i].center));
            h_rec.color = vec_mult(rt.sp[i].col, 255);
            h_rec.center = rt.sp[i].center;
            h_rec.id = i;
            h_rec.shape = 0;
        }
    }
    i = -1;
    while (++i < rt.num_pl)
    {
        t = hit_plane(rt.pl[i].normal, rt.pl[i].point, ray);
        if ((i == 0 && rt.num_sp == 0) || (t > 0.0 && (h_rec.t < 0.0 || t < h_rec.t)))
        {
            h_rec.t = t;
            h_rec.point = ray_point(ray, t);
            h_rec.normal = rt.pl[i].normal;
            h_rec.color = vec_mult(rt.pl[i].col, 255);
            h_rec.center = rt.pl[i].point;
            h_rec.id = i;
            h_rec.shape = 1;
        }
    }
    i = -1;
    while (++i < rt.num_cy)
    {
        t = hit_cylinder(rt.cy[i].center, rt.cy[i].normal, rt.cy[i].radius, rt.cy[i].height, ray);
        if ((i == 0 && (rt.num_sp == 0 && rt.num_pl == 0)) || (t > 0.0 && (h_rec.t < 0.0 || t < h_rec.t)))
        {
            h_rec.t = t;
            h_rec.point = ray_point(ray, t);
            h_rec.normal = cyl_normal(h_rec.point, rt.cy[i].center, rt.cy[i].normal);
            h_rec.color = vec_mult(rt.cy[i].col, 255);
            h_rec.center = rt.cy[i].center;
            h_rec.id = i;
            h_rec.shape = 2;
        }
    }
    if (h_rec.t > 0.0)
    {
        // ambient light
        t_ray shadow_ray;
        t_vector light_dir = unit_vector(vec_sub(rt.l.point, h_rec.point));

        shadow_ray.start = h_rec.point;
        shadow_ray.direction = light_dir;
        int in_shadow = -1;
        i = -1;
        while (++i < rt.num_sp)
        {
            if (hit_sphere(rt.sp[i].center, rt.sp[i].radius, shadow_ray) > 0.0)
            {
                if (h_rec.shape == 0 && h_rec.id == i)
                    continue ;
                in_shadow = 1;
                break ;
            }
        }
        // maybe planes shouldn't cast shadows? XD
        // i = -1;
        // while (in_shadow == -1 && ++i < rt.num_pl)
        // {
        //     if (hit_plane(rt.pl[i].normal, rt.pl[i].point, shadow_ray) > 0.0)
        //     {
        //         if (h_rec.shape == 1 && h_rec.id == i)
        //             continue ;
        //         in_shadow = 1;
        //         break ;
        //     }
        // }
        if (in_shadow == 1)
            return (vec_mult(h_rec.color, 0.4));
        // diffuse light
        t_vector light_color = {1.0, 1.0, 1.0};
        t_vector lighting = {1.0, 1.0, 1.0};

        float diffuse_strength = fmaxf(0.0, dot_prod(light_dir, h_rec.normal));
        t_vector diffuse = vec_mult(light_color, diffuse_strength);

        // specular light
        // t_vector view_source = rt.c.or_vec;
        t_vector reflect_source = unit_vector(reflect(vec_mult(rt.l.point, -1), h_rec.normal));
        float specularStrength = fmaxf(0.0, dot_prod(rt.c.or_vec, reflect_source));
        specularStrength = powf(specularStrength, 32.0);
        t_vector specular = vec_mult(light_color, specularStrength);
        
        lighting = vec_add(vec_mult((t_vector){rt.a.ratio, rt.a.ratio, rt.a.ratio}, 0.8), vec_mult(diffuse, 0.5));
        lighting = vec_add(vec_mult(specular, 0.5), lighting);
        t_vector colour = {h_rec.color.x * lighting.x, h_rec.color.y * lighting.y, h_rec.color.z * lighting.z};
        colour.x = fminf(fmaxf(colour.x, 0.0), 255.0);
        colour.y = fminf(fmaxf(colour.y, 0.0), 255.0);
        colour.z = fminf(fmaxf(colour.z, 0.0), 255.0);
        return (colour);
    }

    return ((t_vector){255, 255, 255});
}

void	set_px_col(mlx_image_t *img, int x, int y, unsigned int color)
{
	int	index;

	index = (y * (int)img->width + x) * 4;
	if (x < (int)img->width && y < (int)img->height)
	{
		img->pixels[index] = (color >> 24) & 0xFF;
		img->pixels[index + 1] = (color >> 16) & 0xFF;
		img->pixels[index + 2] = (color >> 8) & 0xFF;
		img->pixels[index + 3] = color & 0xFF;
	}
}

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}