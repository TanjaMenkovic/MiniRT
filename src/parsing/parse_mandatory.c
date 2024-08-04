#include "../../headers/minirt.h"

int    parse_amb(char **line, t_rt *rt)
{
    t_amb   a;
    int     i;

    i = 0;
    printf("parse amb\n");
    if (rt->a.id != 0)
    {
        error_msg("there can be only 1 ambient lighting in .rt file!\n", 0);
        return (0);
    }
    if (numofel(line, 3) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_ratio(line[1], &a.ratio) == 0)
            return (0);
        if (i == 2 && parse_color(line[2], &a.col) == 0)
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
    printf("parse camera\n");
    if (rt->c.id != 0)
    {
        error_msg("there can be only 1 camera in .rt file!\n", 0);
        return (0);
    }
    if (numofel(line, 4) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_vector(line[1], &c.point) == 0)
            return (0);
        if (i == 2 && parse_normvec(line[2], &c.or_vec) == 0)
            return (0);
        if (i == 3 && parse_view(line[3], &c.fov) == 0)
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
    printf("parse light\n");
    printf("pl1\n");
    if (rt->l.id != 0)
    {
        error_msg("there can be only 1 light in .rt file!\n", 0);
        return (0);
    }
    printf("pl2\n");
    if (numofel(line, 4) == 0)
        return (0);
    printf("pl3\n");
    while (line && line[++i])
    {
        printf("bla\n");
        if (i == 1 && parse_vector(line[1], &l.point) == 0)
            return (0);
        break;
        printf("bla1\n");
        if (i == 2 && parse_float(line[2], &l.bright) == 0)
            return (0);
        printf("bla2\n");
        if (i == 3 && parse_color(line[3], &l.col) == 0)
            return (0);
    }
    printf("pl4\n");
    l.id = 1;
    rt->l = l;
    printf("pl5\n");
    return (1);
}