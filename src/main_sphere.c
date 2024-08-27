#include "../headers/minirt.h"
# define WIDTH 1280
# define HEIGHT 720

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

t_vector ray_color(t_ray ray)
{
    t_vector N;
    float   t;
    float   t_temp;
    int     i;

    i = 0;
    // new struct that has sphere_distance if no hit -1
    // plane distance if hit, and cylinder distance if hit
    // compare all and find closest one that we colour
    float t_temp = hit_sphere((t_vector){0, 0, -1}, 0.5, ray);
    if (t > 0.0)
    {
        N = unit_vector(vec_sub(ray_point(ray, t), (t_vector){0, 0, -1}));
        float angle = dot_prod(N, ray.direction);
        return ((t_vector){0.0, fabs(angle) * 255.0, 0.0});
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

int main()
{
    mlx_t *mlx;
    mlx_image_t *img;
    float aspect_ratio = (float)WIDTH/HEIGHT;

    // camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = 2.0 * aspect_ratio;
    t_vector camera_center = {0, 0, 0};

    // viewport_u
    t_vector viewport_u = {viewport_width, 0, 0};
    // viewport_v
    t_vector viewport_v = {0, -viewport_height, 0};
    // calculate the horizontal and vertical delta vectors from pixel to pixel.
    t_vector pixel_delta_u = vec_div(viewport_u, WIDTH);
    t_vector pixel_delta_v = vec_div(viewport_v, HEIGHT);
    // calculate location of upper left pixel.
    t_vector focal = {0, 0, focal_length};
    t_vector viewport_upper_left = vec_sub(camera_center, focal);
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_u, 2));
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_v, 2));
    t_vector pixel00_loc = vec_mult(vec_add(pixel_delta_u, pixel_delta_v), 0.5);
    pixel00_loc = vec_add(pixel00_loc, viewport_upper_left);


    t_vector color;
    int rgba;
    int x;
    int y;
    
    mlx = mlx_init(WIDTH, HEIGHT, "SPHERE", false);
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    mlx_image_to_window(mlx, img, 0, 0);

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vector pixel_center = vec_add(pixel00_loc, vec_mult(pixel_delta_u, x));
            pixel_center = vec_add(pixel_center, vec_mult(pixel_delta_v, y));
            t_vector ray_direction = vec_sub(pixel_center, camera_center);
            t_ray ray = init_ray(camera_center, ray_direction);
            color = ray_color(ray);
            rgba = get_rgba(color.x, color.y, color.z, 255);
            set_px_col(img, x, y, rgba);
            x++;
        }
        y++;
    }
    mlx_loop(mlx);

    return (0);
}

/*
typedef struct s_sphere
{
    int id;
    t_vector center;
    float radius;
    t_vector col;
}   t_sphere;
*/