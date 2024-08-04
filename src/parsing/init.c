#include "../../headers/minirt.h"

/* 
initialize t_rt at the beginning 
find how many sp, pl and cy we have and init those numbers
allocate memory for each array of sp, pl and cy
put all id to be 0
make initialization for each smaller struct and marge it there
*/

/*
count how many sp, pl and cy are in file and add it to the struct

CHEK IF IT'S CORRECT!
*/
static void    count_str(t_rt *rt, t_index *j, int fd)
{
    char *str;

    init_index(j);
    while ((str = get_next_line(fd)) != NULL)
    {
        ft_num_of_sub(str, "sp", &j->s);
        ft_num_of_sub(str, "pl", &j->p);
        ft_num_of_sub(str, "cy", &j->c);
        free(str);
    }
    rt->num_sp = j->s;
    rt->num_pl = j->p;
    rt->num_cy = j->c;
    init_index(j);
}

void    init_rt(t_rt *rt, t_index *j, char **argv)
{
    int fd;
    printf("init:\n");
    fd = open_file(argv[1]);
    printf("in1\n");
    count_str(rt, j, fd);
    printf("in2\n");
    close (fd);
    printf("in3\n");
    // rt->sp = (t_sphere *)malloc(sizeof(t_sphere) * rt->num_sp);
    // rt->cy = (t_cylinder *)malloc(sizeof(t_cylinder) * rt->num_cy);
    // rt->pl = (t_plane *)malloc(sizeof(t_plane) * rt->num_pl);
    // if (!rt->sp || !rt->cy || !rt->pl)
    // {
    //     free(rt->sp);
    //     free(rt->cy);
    //     free(rt->pl);
    //     malloc_error();
    // }
    init_amb(rt);
    printf("in4\n");
    init_camera(rt);
    printf("in5\n");
    init_light(rt);
    printf("in6\n");
    init_sphere(rt);
    printf("in7\n");
    init_plane(rt);
    printf("in8\n");
    init_cylinder(rt);
    printf("in9\n");
}
