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

void    initialize_camera(t_rt *rt)
{
    float       focal_length;
    t_vector    focal;
    t_vector    viewport_u;
    t_vector    viewport_v;
    t_vector    viewport_upper_left;
    
    focal_length = (WIDTH / 2) / (tanf((rt->c.fov * (PI/180))/2));
    // viewport_u
    viewport_u = (t_vector){VIEWPORT_W, 0, 0};
    // viewport_v, in our viewport y increases upward, in our graphical window
    // y increases downward, hence -VIEWPORT_H
    viewport_v = (t_vector){0, -VIEWPORT_H, 0};

    // calculate the horizontal and vertical delta vectors from pixel to pixel.
    rt->c.pixel_delta_u = vec_div(viewport_u, WIDTH);
    rt->c.pixel_delta_v = vec_div(viewport_v, HEIGHT);
    // calculate location of upper left pixel.
    focal = (t_vector){0, 0, focal_length};
    viewport_upper_left = vec_sub(rt->c.point, focal);
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_u, 2));
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_v, 2));
    rt->c.pixel00_loc = vec_mult(vec_add(rt->c.pixel_delta_u, rt->c.pixel_delta_v), 0.5);
    rt->c.pixel00_loc = vec_add(rt->c.pixel00_loc, viewport_upper_left);
}

void    initialize_mlx(t_rt *rt)
{
    rt->mlx = mlx_init(WIDTH, HEIGHT, "SPHERES", false);
    rt->img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
    mlx_image_to_window(rt->mlx, rt->img, 0, 0);
}

void    render_scene(t_rt rt)
{
    t_vector color;
    int rgba;
    int x;
    int y;

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vector pixel_center = vec_add(rt.c.pixel00_loc, vec_mult(rt.c.pixel_delta_u, x));
            pixel_center = vec_add(pixel_center, vec_mult(rt.c.pixel_delta_v, y));
            t_vector ray_direction = vec_sub(pixel_center, rt.c.point);
            t_ray ray = init_ray(rt.c.point, ray_direction);
            color = ray_color(ray, rt);
            rgba = get_rgba(color.x, color.y, color.z, 255);
            set_px_col(rt.img, x, y, rgba);
            x++;
        }
        y++;
    }
}

int main(int argc, char **argv)
{
    t_rt    rt;
    int     fd;
    t_index j;

    check_args(argc, argv);
    init_rt(&rt, &j, argv);
    fd = open_file(argv[1]);
    // Need to divide diameter by 2 in sphere and cylinder!!!!
    if (parse_rt(&rt, fd, &j) == 0)
    {
        //free everything
        free_all(&rt);
        close(fd);
        return (1);
    }   
    close(fd);

    initialize_camera(&rt);
    initialize_mlx(&rt);
    render_scene(rt);

    mlx_loop(rt.mlx);
    free_all(&rt);
    return (0);
}