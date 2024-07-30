#include "../headers/minirt.h"

/*
check if I want to make it as an array, or I want to make it as list.
*/
int    parse_sphere(char **line, t_rt *rt)
{
    t_sphere    sp;

    if (numofel(line, 4) == 0)
        return (0);
    return (1);
}

int    parse_plane(char **line, t_rt *rt)
{
    t_plane pl;

    if (numofel(line, 4) == 0)
        return (0);
    return (1);
}

int    parse_cylinder(char **line, t_rt *rt)
{
    t_cylinder  cy;

    if (numofel(line, 6) == 0)
        return (0);
    return (1);
}