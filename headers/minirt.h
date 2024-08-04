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
    int id;
    float ratio;
    t_vector col;
}   t_amb;

/* C 0,0,0 0,0,1 70 */
typedef struct s_camera
{
    int id;
    t_vector point; //view_point
    t_vector or_vec; //orientation vector
    float fov; // Horizontal field of view 
}   t_camera;

/* L -40.0,50.0,0.0 0.6 10,0,255 */
typedef struct s_light
{
    int id;
    t_vector point; //light point
    float bright; //brightness ratio
    t_vector col; //color
}   t_light;

/* sp 0,0,20 10 255,0,0 */
typedef struct s_sphere
{
    int id;
    t_vector center;
    float radius;
    t_vector col;
}   t_sphere;

/* pl 0,-10,0 0,1,0 0,255,0 */
typedef struct s_plane
{
    int id;
    t_vector point;
    t_vector normal; //3d normalized normal vector
    t_vector col;
}   t_plane;

/* cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255 */
typedef struct s_cylinder
{
    int id;
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

typedef struct s_index
{
    int s;
    int p;
    int c;
}   t_index;

/* files.c */
int open_file(char *file);

/* parse_utils.c */
int parse_ratio(char *str, float *num);
int parse_view(char *str, float *num);
int parse_normvec(char *str, t_vector *v);
int parse_color(char *str, t_vector *v);

/* parse_utils1.c */
int parse_float(char *str, float *num);
int parse_ulong(char *str, size_t *num);
int parse_vector(char *str, t_vector *v);

/* parse_mandatory.c */
int       parse_amb(char **line, t_rt *rt);
int    parse_camera(char **line, t_rt *rt);
int     parse_light(char **line, t_rt *rt);

/* parse_objects.c */
int    parse_sphere(char **line, t_rt *rt, int id);
int     parse_plane(char **line, t_rt *rt, int id);
int  parse_cylinder(char **line, t_rt *rt, int id);

/* rtfile.c */
void init_index(t_index *j);
int        parse_rt(t_rt *rt, int fd, t_index *j);

/* init.c */
void    init_rt(t_rt *rt, t_index *j, char **argv);

/* init1.c */
void    init_vector(t_vector *v);
void    init_amb(t_rt *rt);
void    init_camera(t_rt *rt);
void    init_light(t_rt *rt);
void    init_sphere(t_rt *rt);
void    init_plane(t_rt *rt);
void    init_cylinder(t_rt *rt);

#endif