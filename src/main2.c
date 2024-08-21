#include "../headers/minirt.h"

static void check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        error_msg("number of arguments is not valid!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
    if (ft_strchr(argv[1], '.') == NULL || ft_strcmp(ft_strchr(argv[1], '.'), ".rt") != 0)
    {
        error_msg("argument is not correct!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
}

void print_vector(const char *name, t_vector v) {
    printf("%s: (%f, %f, %f)\n", name, v.x, v.y, v.z);
}

void print_rt(t_rt *rt) {
    int i;

    // Print ambient light
    printf("Ambient Light:\n");
    printf("ID: %d\n", rt->a.id);
    printf("Ratio: %f\n", rt->a.ratio);
    print_vector("Color", rt->a.col);

    // Print camera
    printf("\nCamera:\n");
    printf("ID: %d\n", rt->c.id);
    print_vector("View Point", rt->c.point);
    print_vector("Orientation Vector", rt->c.or_vec);
    printf("FOV: %f\n", rt->c.fov);

    // Print light
    printf("\nLight:\n");
    printf("ID: %d\n", rt->l.id);
    print_vector("Point", rt->l.point);
    printf("Brightness: %f\n", rt->l.bright);
    print_vector("Color", rt->l.col);

    // Print spheres
    printf("\nSpheres: %d\n", rt->num_sp);
    for (i = 0; i < rt->num_sp; i++) {
        printf("\nSphere %d:\n", i);
        printf("ID: %d\n", rt->sp[i].id);
        print_vector("Center", rt->sp[i].center);
        printf("Radius: %f\n", rt->sp[i].radius);
        print_vector("Color", rt->sp[i].col);
    }

    // Print planes
    printf("\nPlanes: %d\n", rt->num_pl);
    for (i = 0; i < rt->num_pl; i++) {
        printf("\nPlane %d:\n", i);
        printf("ID: %d\n", rt->pl[i].id);
        print_vector("Point", rt->pl[i].point);
        print_vector("Normal", rt->pl[i].normal);
        print_vector("Color", rt->pl[i].col);
    }

    // Print cylinders
    printf("\nCylinders: %d\n", rt->num_cy);
    for (i = 0; i < rt->num_cy; i++) {
        printf("\nCylinder %d:\n", i);
        printf("ID: %d\n", rt->cy[i].id);
        print_vector("Center", rt->cy[i].center);
        print_vector("Normal", rt->cy[i].normal);
        printf("Radius: %f\n", rt->cy[i].radius);
        printf("Height: %f\n", rt->cy[i].height);
        print_vector("Color", rt->cy[i].col);
    }
}

/*
    I = incoming vector
    N = normal vector, vector perpendicular to hit point
    I - 2.0 * dot(N, I) * N.
*/
t_vector reflect(t_vector I, t_vector N)
{
    t_vector norm;
    t_vector reflection;

    norm = unit_vector(N);
    reflection = vec_mult(norm, dot_prod(norm, I) * 2.0);
    reflection = vec_sub(I, reflection);
    return (reflection);
}

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

float hit_cylinder(t_vector base_center, t_vector axis_dir, float radius, float height, t_ray ray)
{
    // Normalize axis_dir
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
        return (-1.0f);

    // Find the nearest valid intersection with the infinite cylinder
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    float t_cylinder = -1.0f;

    // Calculate the intersection points in 3D space for valid t1 and t2
    if (t1 > 0.0f)
    {
        t_vector p1 = vec_add(ray.start, vec_mult(ray.direction, t1));
        float z1 = dot_prod(vec_sub(p1, base_center), axis_dir);
        if (z1 >= 0.0f && z1 <= height)
            t_cylinder = t1;
    }

    if (t_cylinder < 0.0f && t2 > 0.0f) // Only check t2 if t1 was invalid
    {
        t_vector p2 = vec_add(ray.start, vec_mult(ray.direction, t2));
        float z2 = dot_prod(vec_sub(p2, base_center), axis_dir);
        if (z2 >= 0.0f && z2 <= height)
            t_cylinder = t2;
    }

    // Check intersection with the caps
    float t_cap_top = -1.0f;
    float t_cap_bottom = -1.0f;

    // Top cap plane
    t_vector top_cap_center = vec_add(base_center, vec_mult(axis_dir, height));
    t_cap_top = hit_plane(axis_dir, top_cap_center, ray);

    // Check if the intersection point lies within the top cap's radius
    if (t_cap_top > 0.0f)
    {
        t_vector point_top = vec_add(ray.start, vec_mult(ray.direction, t_cap_top));
        t_vector vec_top = vec_sub(point_top, top_cap_center);
        if (dot_prod(vec_top, vec_top) <= radius * radius)
            t_cylinder = (t_cylinder < 0.0f || t_cap_top < t_cylinder) ? t_cap_top : t_cylinder;
    }

    // Bottom cap plane
    t_cap_bottom = hit_plane(axis_dir, base_center, ray);

    // Check if the intersection point lies within the bottom cap's radius
    if (t_cap_bottom > 0.0f)
    {
        t_vector point_bottom = vec_add(ray.start, vec_mult(ray.direction, t_cap_bottom));
        t_vector vec_bottom = vec_sub(point_bottom, base_center);
        if (dot_prod(vec_bottom, vec_bottom) <= radius * radius)
            t_cylinder = (t_cylinder < 0.0f || t_cap_bottom < t_cylinder) ? t_cap_bottom : t_cylinder;
    }

    return t_cylinder;
}

t_vector cyl_normal(t_vector point, t_vector base_center, t_vector axis_dir)
{
    // Normalize the cylinder's axis direction
    axis_dir = unit_vector(axis_dir);

    // Vector from base center to the intersection point
    t_vector p_minus_c = vec_sub(point, base_center);

    // Project this vector onto the cylinder's axis
    t_vector proj_p_minus_c = vec_mult(axis_dir, dot_prod(p_minus_c, axis_dir));

    // Subtract the projection from p_minus_c to get the perpendicular component
    t_vector normal = vec_sub(p_minus_c, proj_p_minus_c);

    // Normalize the resulting vector to get the normal
    normal = unit_vector(normal);

    return normal;
}

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

int main(int argc, char **argv)
{
    t_rt    rt;
    int     fd;
    t_index j;

    check_args(argc, argv);
    init_rt(&rt, &j, argv);
    fd = open_file(argv[1]);
    if (parse_rt(&rt, fd, &j) == 0)
    {
        //free everything
        close(fd);
        return (1);
    }
    print_rt(&rt);
    
    close(fd);
    // camera
    float focal_length = 1.0;
    // t_vector camera_center = {0, 0, 0};

    // viewport_u
    t_vector viewport_u = {VIEWPORT_W, 0, 0};
    // viewport_v, in our viewport y increases upward, in our graphical window
    // y increases downward, hence -VIEWPORT_H
    t_vector viewport_v = {0, -VIEWPORT_H, 0};
    // calculate the horizontal and vertical delta vectors from pixel to pixel.
    t_vector pixel_delta_u = vec_div(viewport_u, WIDTH);
    t_vector pixel_delta_v = vec_div(viewport_v, HEIGHT);
    // calculate location of upper left pixel.
    t_vector focal = {0, 0, focal_length};
    t_vector viewport_upper_left = vec_sub(rt.c.point, focal);
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_u, 2));
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_v, 2));
    t_vector pixel00_loc = vec_mult(vec_add(pixel_delta_u, pixel_delta_v), 0.5);
    pixel00_loc = vec_add(pixel00_loc, viewport_upper_left);

    t_vector color;
    int rgba;
    int x;
    int y;
    
    rt.mlx = mlx_init(WIDTH, HEIGHT, "SPHERES", false);
    rt.img = mlx_new_image(rt.mlx, WIDTH, HEIGHT);
    mlx_image_to_window(rt.mlx, rt.img, 0, 0);

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vector pixel_center = vec_add(pixel00_loc, vec_mult(pixel_delta_u, x));
            pixel_center = vec_add(pixel_center, vec_mult(pixel_delta_v, y));
            t_vector ray_direction = vec_sub(pixel_center, rt.c.point);
            t_ray ray = init_ray(rt.c.point, ray_direction);
            color = ray_color(ray, rt);
            rgba = get_rgba(color.x, color.y, color.z, 255);
            set_px_col(rt.img, x, y, rgba);
            x++;
        }
        y++;
    }
    mlx_loop(rt.mlx);
    return (0);
}