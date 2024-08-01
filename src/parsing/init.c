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
void    count_str(t_rt *rt, int fd, t_index *j)
{
    char *str;
    int     i;

    while (fd)
    {
        i = 0;
        str = get_next_line(fd);
        ft_num_of_sub(str, "sp", j->s);
        ft_num_of_sub(str, "pl", j->p);
        ft_num_of_sub(str, "cy", j->c);
        free(str);
    }
    rt->num_sp = j->s;
    rt->num_pl = j->p;
    rt->num_cy = j->c;
    init_index(j);
}


