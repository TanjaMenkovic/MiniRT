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
static void    count_str(t_rt *rt, t_index *j, char **argv)
{
    char *str;
    int     fd;

    init_index(j);
    fd = open_file(argv[1]);
    while (fd)
    {
        str = get_next_line(fd);
        ft_num_of_sub(str, "sp", &j->s);
        ft_num_of_sub(str, "pl", &j->p);
        ft_num_of_sub(str, "cy", &j->c);
        free(str);
    }
    rt->num_sp = j->s;
    rt->num_pl = j->p;
    rt->num_cy = j->c;
    init_index(j);
    close(fd);
}

void    init_rt(t_rt *rt, t_index *j, char **argv)
{
    rt = (t_rt *)malloc(sizeof(t_rt));
    count_str(rt, j, argv);
    rt->a.id = 0;
    rt->c.id = 0;
    rt->l.id = 0;
    rt->sp = (t_sphere *)malloc(sizeof(t_sphere) * rt->num_sp);
    rt->cy = (t_cylinder *)malloc(sizeof(t_cylinder) * rt->num_cy);
    rt->pl = (t_plane *)malloc(sizeof(t_plane) * rt->num_pl);
}
