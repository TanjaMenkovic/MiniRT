#include "../../headers/minirt.h"

/*
count how many sp, pl and cy are in file and add it to the struct
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
    fd = open_file(argv[1]);
    count_str(rt, j, fd);
    close (fd);
    init_amb(rt);
    init_camera(rt);
    init_light(rt);
    init_sphere(rt);
    init_plane(rt);
    init_cylinder(rt);
}
