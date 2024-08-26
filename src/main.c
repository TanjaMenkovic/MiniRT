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
/*
    focal_length = distance from camera to viewport

    camera_forward given from the scene rt->c.or_vec

    camera right we get by calculating the cross product of {0, 1, 0} vector
    and camera_forward.

    camera up is calculated from the crossproduct of camera right and camera forward

    viewport_u = vector of whole width of viewport
    viewport_v = vector of whole height of viewport

    rt->c.pixel00_loc = our topleft corner pixel on our viewport
*/
void    initialize_camera(t_rt *rt)
{
    t_init_cam i;

    i.focal_length = (rt->width / 2) / (tanf((rt->c.fov * (PI/180))/2));
    i.viewport_width = rt->width*2;
    i.viewport_height = i.viewport_width/ASPECT_RATIO;
    i.camera_forward = unit_vector(rt->c.or_vec);
    if (vec_len(cross_prod((t_vector){0, 1, 0}, i.camera_forward)) != 0)
        i.camera_right = unit_vector(cross_prod((t_vector){0, 1, 0}, i.camera_forward));
    else
        i.camera_right = unit_vector(cross_prod((t_vector){0, 1, -0.00001}, i.camera_forward));
    i.camera_up = cross_prod(i.camera_forward, i.camera_right);
    i.viewport_u = vec_mult(i.camera_right, i.viewport_width);
    i.viewport_v = vec_mult(i.camera_up, i.viewport_height);
    rt->c.pixel_delta_u = vec_div(i.viewport_u, rt->width);
    rt->c.pixel_delta_v = vec_div(i.viewport_v, rt->height);
    i.focal = vec_mult(i.camera_forward, i.focal_length);
    i.viewport_upper_left = vec_sub(rt->c.point, i.focal);
    i.viewport_upper_left = vec_sub(i.viewport_upper_left, vec_div(i.viewport_u, 2));
    i.viewport_upper_left = vec_sub(i.viewport_upper_left, vec_div(i.viewport_v, 2));
    rt->c.pixel00_loc = vec_mult(vec_add(rt->c.pixel_delta_u, rt->c.pixel_delta_v), 0.5);
    rt->c.pixel00_loc = vec_add(rt->c.pixel00_loc, i.viewport_upper_left);
}

static void	close_hook(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
}

void    render_scene(t_rt rt);

static void resize_callback(int32_t width, int32_t height, void *param)
{
    t_rt *rt;
    
    rt = param;
    rt->width = width;
    rt->height = height;
    mlx_delete_image(rt->mlx, rt->img);
    rt->img = mlx_new_image(rt->mlx, rt->width, rt->height);
    render_scene(*rt);
    mlx_image_to_window(rt->mlx, rt->img, 0, 0);
}


void    initialize_mlx(t_rt *rt)
{
    rt->mlx = mlx_init(rt->width, rt->height, "miniRT", true);
    rt->img = mlx_new_image(rt->mlx, rt->width, rt->height);
    mlx_image_to_window(rt->mlx, rt->img, 0, 0);
    mlx_loop_hook(rt->mlx, &close_hook, rt->mlx);
    mlx_resize_hook(rt->mlx, &resize_callback, rt);
}

void    render_scene(t_rt rt)
{
    t_vector color;
    int rgba;
    int x;
    int y;

    y = -1;
    while (++y < rt.height)
    {
        x = -1;
        while (++x < rt.width)
        {
            t_vector pixel_center = vec_add(rt.c.pixel00_loc, vec_mult(rt.c.pixel_delta_u, x));
            pixel_center = vec_add(pixel_center, vec_mult(rt.c.pixel_delta_v, y));
            t_vector ray_direction = unit_vector(vec_sub(pixel_center, rt.c.point));
            if (dot_prod(ray_direction, rt.c.or_vec) < 0)
            {
                ray_direction = vec_mult(ray_direction, -1.0);
            }
            t_ray ray = init_ray(rt.c.point, ray_direction);
            color = ray_color(ray, rt);
            rgba = get_rgba(color.x, color.y, color.z, 255);
            set_px_col(rt.img, x, y, rgba);
        }
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