#include "../../headers/minirt.h"

/* 
we need to see how actually this .rt file looks
and can it have also some random lines in it, or everything 
needs to be properly written.  
also can we have empty lines or not and so on.
we should use getnextline to read each line and based on line 
parse it.
we need to check if we need another split for first splitting it
based on white spaces, or it can be splited only based on space.
*/

/*
parsing functions are returning 1 in case nothing fails, and 0 if 
there are some errors
*/

void init_index(t_index *j)
{
    j->c = 0;
    j->p = 0;
    j->s = 0;
}

static  parse_line(char **line, t_rt *rt, t_index *j, int *i)
{
    if (ft_strcmp(line[0], "A") == 0)
        *i = parse_amb(line, rt);
    else if (ft_strcmp(line[0], "C") == 0)
        *i = parse_camera(line, rt);
    else if (ft_strcmp(line[0], "L") == 0)
        *i = parse_light(line, rt);
    else if (ft_strcmp(line[0], "sp") == 0)
        *i = parse_sphere(line, rt, (j->s)++);
    else if (ft_strcmp(line[0], "pl") == 0)
        *i = parse_plane(line, rt, (j->p)++);
    else if (ft_strcmp(line[0], "cy") == 0)
        *i = parse_cylinder(line, rt, (j->c)++);
    else if (line[0])
    {
        *i = 0;
        error_msg("incorrect value in .rt file!\n", "Correct starting with: A/C/L/sp/pl/cy");
    }
}

/* checking if we have ambient lighting, camera and light in our file */
static int  check_mandatory(t_rt *rt)
{
    if (rt->a.id == 0)
    {
        error_msg("missing ambient lighting!\n", 0);
        return (0);
    }
    if (rt->c.id == 0)
    {
        error_msg("missing camera!\n", 0);
        return (0);
    }
    if (rt->l.id == 0)
    {
        error_msg("missing alight!\n", 0);
        return (0);
    }
    return (1);
}

int    parse_rt(t_rt *rt, int fd, t_index *j)
{
    char    **line;
    char    *str;
    int     i;
    
    while (fd)
    {
        i = 1;
        str = get_next_line(fd);
        line = ft_splitset(str, " \n\t\r\v\f");
        free(str);
        parse_line(line, rt, j, &i);
        if (i == 0)
        {
            free_arr(line);
            close (fd);
            return (0);
        }
        free_arr(line);
    }
    close(fd);
    if (check_mandatory(rt) == 0)
        return (0);
    return (1);
}
