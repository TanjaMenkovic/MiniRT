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

 //   *rt = (t_rt *)malloc(sizeof(t_rt));
    fd = open_file(argv[1]);
    count_str(rt, j, fd);
    close (fd);
    rt->a.id = 0;
    rt->c.id = 0;
    rt->l.id = 0;
    rt->sp = (t_sphere *)malloc(sizeof(t_sphere) * rt->num_sp);
    rt->cy = (t_cylinder *)malloc(sizeof(t_cylinder) * rt->num_cy);
    rt->pl = (t_plane *)malloc(sizeof(t_plane) * rt->num_pl);
    if (!rt->sp || !rt->cy || !rt->pl)
    {
        free(rt->sp);
        free(rt->cy);
        free(rt->pl);
        malloc_error();
    }
}
