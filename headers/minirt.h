#ifndef MINIRT_H
# define MINIRT_H

# include "errors.h"
# include "utils.h"

# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>

/* A 0.2 255,255,255 */
typedef struct s_amb
{
    float ratio;
    t_vector col;
}   t_amb;

/* C 0,0,0 0,0,1 70 */
typedef struct s_camera
{
    t_vector point; //view_point
    t_vector or_vec; //orientation vector
    float fov; // Horizontal field of view 
}   t_camera;

/* L -40.0,50.0,0.0 0.6 10,0,255 */
typedef struct s_light
{
    t_vector point; //light point
    float bright; //brightness ratio
    t_vector col; //color
}   t_light;

/* sp 0,0,20 10 255,0,0 */
typedef struct s_sphere
{
    t_vector center;
    float radius;
    t_vector col;
}   t_sphere;

/* pl 0,-10,0 0,1,0 0,255,0 */
typedef struct s_plane
{
    t_vector point;
    t_vector normal; //3d normalized normal vector
    t_vector col;
}   t_plane;

/* cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255 */
typedef struct s_cylinder
{
    t_vector center;
    t_vector normal;
    float radius;
    float hight;
    t_vector col;
}   t_cylinder;

/* infrmations from .rt file */
typedef struct s_rt
{
    t_amb a;
    t_camera c;
    t_light l;
    t_sphere *sp;
    t_plane *pl;
    t_cylinder *cy;
    int num_sp;
    int num_pl;
    int num_cy;

}   t_rt;

/* files.c */
int open_file(char *file);

/* rtfile.c */
t_amb       parse_amb(char **line, t_rt *rt);
t_camera    parse_camera(char **line, t_rt *rt);
t_light     parse_light(char **line, t_rt *rt);
t_sphere    parse_sphere(char **line, t_rt *rt);
t_plane     parse_plane(char **line, t_rt *rt);
t_cylinder  parse_cylinder(char **line, t_rt *rt);
t_rt        parse_rt(char *file);

#endif