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
        printf("Height: %f\n", rt->cy[i].hight);
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
        if (i == 0 || (t > 0.0 && (h_rec.t < 0.0 || t < h_rec.t)))
        {
            h_rec.t = t;
            h_rec.point = ray_point(ray, t);
            h_rec.normal = unit_vector(vec_sub(h_rec.point, rt.sp[i].center));
            h_rec.color = vec_mult(rt.sp[i].col, 255);
            h_rec.center = rt.sp[i].center;
        }
    }
    if (h_rec.t > 0.0)
    {
        // ambient light
        t_vector ambient = {0.5, 0.5, 0.5};

        t_ray shadow_ray;
        t_vector light_source = {3.0, 0.0, -1.0};
        t_vector light_dir = unit_vector(vec_sub(light_source, h_rec.point));

        shadow_ray.start = h_rec.point;
        shadow_ray.direction = light_dir;
        int in_shadow = -1;
        i = -1;
        while (++i < rt.num_sp)
        {
            if (hit_sphere(rt.sp[i].center, rt.sp[i].radius, shadow_ray) > 0.0)
            {
                if (rt.sp[i].center.x == h_rec.center.x && rt.sp[i].center.y == h_rec.center.y && rt.sp[i].center.z == h_rec.center.z)
                    continue ;
                in_shadow = 1;
                break ;
            }
        }
        if (in_shadow == 1)
            return (vec_mult(h_rec.color, 0.4));
        // diffuse light
        t_vector light_color = {1.0, 1.0, 1.0};
        t_vector lighting = {1.0, 1.0, 1.0};

        float diffuse_strength = fmaxf(0.0, dot_prod(light_dir, h_rec.normal));
        t_vector diffuse = vec_mult(light_color, diffuse_strength);

        // specular light
        t_vector view_source = rt.c.or_vec;
        t_vector reflect_source = unit_vector(reflect(vec_mult(light_source, -1), h_rec.normal));
        float specularStrength = fmaxf(0.0, dot_prod(view_source, reflect_source));
        specularStrength = powf(specularStrength, 32.0);
        t_vector specular = vec_mult(light_color, specularStrength);
        
        lighting = vec_add(vec_mult(ambient, 1.0), vec_mult(diffuse, 0.5));
        lighting = vec_add(vec_mult(specular, 0.5), lighting);
        t_vector colour = {h_rec.color.x * lighting.x, h_rec.color.y * lighting.y, h_rec.color.z * lighting.z};
        colour.x = fminf(fmaxf(colour.x, 0.0), 255.0);
        colour.y = fminf(fmaxf(colour.y, 0.0), 255.0);
        colour.z = fminf(fmaxf(colour.z, 0.0), 255.0);
        return (colour);
    }

    t_vector white = {255.0, 255.0, 255.0};
    t_vector blue = {0.0, 0.0, 255.0};

    t_vector unit_direction = unit_vector(ray.direction);
    float a = 0.5 * (unit_direction.y + 1.0);
    return (vec_add(vec_mult(white ,(1.0 - a)), vec_mult(blue, a)));
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