/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:40:36 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 13:52:22 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"

# include "errors.h"
# include "utils.h"

# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <math.h>

/* Constants */
# define WIDTH 1280
# define HEIGHT 720
# define ASPECT_RATIO ((float)WIDTH/HEIGHT)
# define VIEWPORT_H  ((float)2.0)
# define VIEWPORT_W  ((float)2.0 * ASPECT_RATIO)
# define PI 3.1415927f

/* A 0.2 255,255,255 */
typedef struct s_amb
{
	int			id;
	float		ratio;
	t_vector	col;
}	t_amb;

typedef struct s_init_cam
{
	float		focal_length;
	t_vector	focal;
	t_vector	viewport_u;
	t_vector	viewport_v;
	t_vector	viewport_upper_left;
	float		viewport_width;
	float		viewport_height;
	t_vector	camera_forward;
	t_vector	camera_right;
	t_vector	camera_up;
}	t_init_cam;

/*
point=view_point
or_vec=orientation vector
fov=Horizontal field of view
*/
typedef struct s_camera
{
	int			id;
	t_vector	point;
	t_vector	or_vec;
	float		fov;
	t_vector	pixel00_loc;
	t_vector	pixel_delta_u;
	t_vector	pixel_delta_v;
}	t_camera;

/* 
L -40.0,50.0,0.0 0.6 10,0,255 

point=light point
bright=brightness ratio
col=color
*/
typedef struct s_light
{
	int			id;
	t_vector	point;
	float		bright;
	t_vector	col;
	t_vector	light_dir;
	t_vector	light_color;
	t_vector	lighting;
	t_ray		shadow_ray;
}	t_light;

/* 
sp 0,0,20 10 255,0,0 
*/
typedef struct s_sphere
{
	int			id;
	t_vector	center;
	float		radius;
	t_vector	col;
}	t_sphere;

/* 
pl 0,-10,0 0,1,0 0,255,0 

normal=3d normalized normal vector
*/
typedef struct s_plane
{
	int			id;
	t_vector	point;
	t_vector	normal;
	t_vector	col;
}	t_plane;

/* 
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255 
*/
typedef struct s_cylinder
{
	int			id;
	t_vector	center;
	t_vector	normal;
	float		radius;
	float		height;
	t_vector	col;
}	t_cylinder;

typedef struct s_cyl_body
{
	float		t_cylinder;
	t_vector	oc;
	t_vector	oc_perp;
	t_vector	ray_dir_perp;
	float		a;
	float		b;
	float		c;
	float		discriminant;
	float		sqrt_discriminant;
	float		t1;
	float		t2;
	t_vector	p1;
	t_vector	p2;
	float		z1;
	float		z2;
}	t_cyl_body;

/* infrmations from .rt file */
typedef struct s_rt
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_amb		a;
	t_camera	c;
	t_light		l;
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;
	int			width;
	int			height;
	int			num_sp;
	int			num_pl;
	int			num_cy;
}	t_rt;

typedef struct s_index
{
	int	s;
	int	p;
	int	c;
}	t_index;

/* files.c */
int			open_file(char *file);

/* parse_utils.c */
int			parse_ratio(char *str, float *num);
int			parse_view(char *str, float *num);
int			parse_normvec(char *str, t_vector *v);
int			parse_color(char *str, t_vector *v);

/* parse_utils1.c */
int			parse_float(char *str, float *num);
int			parse_ulong(char *str, size_t *num);
int			parse_vector(char *str, t_vector *v);

/* parse_mandatory.c */
int			parse_amb(char **line, t_rt *rt);
int			parse_camera(char **line, t_rt *rt);
int			parse_light(char **line, t_rt *rt);

/* parse_objects.c */
int			parse_sphere(char **line, t_rt *rt, int id);
int			parse_plane(char **line, t_rt *rt, int id);
int			parse_cylinder(char **line, t_rt *rt, int id);

/* rtfile.c */
void		init_index(t_index *j);
int			parse_rt(t_rt *rt, int fd, t_index *j);

/* init.c */
void		init_rt(t_rt *rt, t_index *j, char **argv);
void		init_vector(t_vector *v);
void		init_amb(t_rt *rt);
void		init_camera(t_rt *rt);

/* init1.c */
void		init_light(t_rt *rt);
void		init_sphere(t_rt *rt);
void		init_plane(t_rt *rt);
void		init_cylinder(t_rt *rt);

/* freeing.c */
void		free_all(t_rt *rt);

/* utils_figures.c */
t_vector	reflect(t_vector I, t_vector N);

/* sphere.c */
float		hit_sphere(t_vector center, float radius, t_ray ray);

/* plane.c */
float		hit_plane(t_vector normal, t_vector point, t_ray ray);

/* cylinder.c */
float		hit_cylinder(t_vector base_center, t_vector axis_dir, float radius, float height, t_ray ray);
t_vector	cyl_normal(t_vector point, t_vector base_center, t_vector axis_dir, float height, float radius);

/* pixel_color.c */
void		set_px_col(mlx_image_t *img, int x, int y, unsigned int color);
int			get_rgba(int r, int g, int b, int a);

/* color.c */
t_vector	ray_color(t_ray ray, t_rt rt);
void		set_px_col(mlx_image_t *img, int x, int y, unsigned int color);
int			get_rgba(int r, int g, int b, int a);

/* shadow.c */
void		check_shadow(t_rt rt, int *in_shadow, t_hit_record h_rec);

#endif