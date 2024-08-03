#include "../../headers/minirt.h"

/*
check if I want to make it as an array, or I want to make it as list.
*/
int    parse_sphere(char **line, t_rt *rt, int id)
{
    t_sphere    sp;
    int         i;

    i = 0;
    printf("sp1\n");
    if (numofel(line, 4) == 0)
        return (0);
    printf("sp2\n");
    while (line && line[++i])
    {
        printf("sp3\n");
        if (i == 1 && parse_vector(line[i], &sp.center) == 0)
            return (0);
        if (i == 2 && parse_ratio(line[i], &sp.radius) == 0)
            return (0);
        if (i == 3 && parse_color(line[i], &sp.col) == 0)
            return (0);
        printf("sp4\n");
    }
    printf("sp5\n");
    sp.id = id;
    if (id < rt->num_sp)
        rt->sp[id] = sp;
    else
        return (0);
    printf("sp6\n");
    return (1);
}

int    parse_plane(char **line, t_rt *rt, int id)
{
    t_plane pl;
    int     i;

    i = 0;
    if (numofel(line, 4) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_vector(line[i], &pl.point) == 0)
            return (0);
        if (i == 2 && parse_normvec(line[i], &pl.normal) == 0)
            return (0);
        if (i == 3 && parse_color(line[i], &pl.col) == 0)
            return (0);
    }
    pl.id = id;
    if (id < rt->num_pl)
        rt->pl[id] = pl;
    else
        return (0);
    return (1);
}

int    parse_cylinder(char **line, t_rt *rt, int id)
{
    t_cylinder  cy;
    int         i;

    i = 0;
    if (numofel(line, 6) == 0)
        return (0);
    while (line && line[++i])
    {
        if (i == 1 && parse_vector(line[i], &cy.center) == 0)
            return (0);
        if (i == 2 && parse_normvec(line[i], &cy.normal) == 0)
            return (0);
        if (i == 3 && parse_float(line[i], &cy.radius) == 0)
            return (0);
        if (i == 4 && parse_float(line[i], &cy.hight) == 0)
            return (0);
        if (i == 5 && parse_color(line[i], &cy.col) == 0)
            return (0);
    }
    cy.id = id;
    if (id < rt->num_cy)
        rt->cy[id] = cy;
    else
        return (0);
    return (1);
}