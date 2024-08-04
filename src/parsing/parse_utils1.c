#include "../../headers/minirt.h"

int parse_ulong(char *str, size_t *num)
{
    if (is_ulong(str) == 0)
    {
        error_msg("not a number!\n", 0);
        return (0);
    }
    *num = (size_t)ft_atoi(str);
    return (1);
}

int parse_float(char *str, float *num)
{
    if (is_float(str) == 0)
    {
        error_msg("not a number!\n", 0);
        return (0);
    }
    *num = ft_atof(str);
    return (1);
}
int parse_vector(char *str, t_vector *v) //any float values
{
    char    **vec;

    printf("parsing vector\n");
    printf("string: '%s'\n", str);
    int i = 0;
    while(1)
    {
        printf("%c", str[i]);
        if (str[i] == 0)
            break ;
        i++;
    }
    vec = ft_split(str, ',');
    printf("pv\n");
    print_doublearr(vec);
    printf("pv1\n");
    if (doublelen(vec) != 3)
    {
        error_msg("vector should have only 3 coordinates!\n", 0);
        free_arr(vec);
        return (0);
    }
    printf("pv2\n");
    if (is_float(vec[0]) == 0 || is_float(vec[1]) == 0 || is_float(vec[2]) == 0)
    {
        error_msg("not a number!\n", 0);
        free_arr(vec);
        return (0);
    }
    printf("pv3\n");
    v->x = ft_atof(vec[0]);
    v->y = ft_atof(vec[1]);
    v->z = ft_atof(vec[2]);
    free_arr(vec);
    printf("pv4\n");
    return (1);
}