#include "../headers/minirt.h"

int    parse_amb(char **line, t_rt *rt)
{
    t_amb   a;
    int     i;

    i = 0;
    if (rt->a.id != 0)
    {
        error_msg("there can be only 1 ambient lighting in .rt file!\n", 0);
        return (0);
    }
    if (numofel(line, 3) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_ratio(line[i], &a.ratio) == 0)
            return (0);
        if (i == 2 && parse_color(line[i], &a.col) == 0)
            return (0);
    }
    a.id = 1;
    rt->a = a;
    return (1);
}

int    parse_camera(char **line, t_rt *rt)
{
    t_camera       c;
    int     i;

    i = 0;
    if (rt->c.id != 0)
    {
        error_msg("there can be only 1 camera in .rt file!\n", 0);
        return (0);
    }
    if (numofel(line, 4) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_vector(line[i], &c.point) == 0)
            return (0);
        if (i == 2 && parse_normvec(line[i], &c.or_vec) == 0)
            return (0);
        if (i == 3 && parse_view(line[i], &c.fov) == 0)
            return (0);
    }
    c.id = 1;
    rt->c = c;
    return (1);
}

int    parse_light(char **line, t_rt *rt)
{
    t_light l;
    int     i;

    i = 0;
    if (rt->l.id != 0)
    {
        error_msg("there can be only 1 light in .rt file!\n", 0);
        return (0);
    }
    if (numofel(line, 4) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_vector(line[i], &l.point) == 0)
            return (0);
        if (i == 2 && parse_float(line[i], &l.bright) == 0)
            return (0);
        if (i == 3 && parse_color(line[i], &l.col) == 0)
            return (0);
    }
    l.id = 1;
    rt->l = l;
    return (1);
}