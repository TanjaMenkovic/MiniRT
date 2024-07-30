#include "../headers/minirt.h"

int parse_ratio(char *str, float *num)
{
    if (parse_float(str, num) == 0)
        return (0);
    if (*num < 0.0 || *num > 1.0)
    {
        error_msg("ratio needs to be in interval [0,1]!\n", 0);
        return (0);
    }
    return (1);
}

int parse_view(char *str, float *num)
{
    if (parse_float(str, num) == 0)
        return (0);
    if (*num < 0.0 || *num > 180.0)
    {
        error_msg("field view needs to be in interval [0,180]!\n", 0);
        return (0);
    }
    return (1);
}

int parse_normvec(char *str, t_vector *v) // float values between [-1,1]
{
    if (parse_vector(str, v) == 0)
        return (0);
    if (v->x < -1 || v->x > 1 || v->y < -1 || v->y > 1 || v->z < -1 || v->z > 1)
    {
        error_msg("vector's values need to be in interval [-1,1]!\n", 0);
        return (0);
    }
    return (1);
}

int parse_color(char *str, t_vector *v) // values from [0,255] divided by 255, to get normalized values
{
    char    **vec;
    int     i;

    i = -1;
    vec = ft_split(str, ',');
    if (doublelen(vec) != 3)
    {
        error_msg("vector should have only 3 coordinates!\n", 0);
        free_arr(vec);
        return (0);
    }
    if (is_ulong(vec[0]) == 0 || is_ulong(vec[1]) == 0 || is_ulong(vec[2]) == 0)
    {
        error_msg("not a number!\n", 0);
        free_arr(vec);
        return (0);
    }
    if (is_color(vec) == 0)
        return (0);
    v->x = (float) ft_atoi(vec[0]) / 255;
    v->y = (float) ft_atoi(vec[1]) / 255;
    v->z = (float) ft_atoi(vec[2]) / 255;
    free_arr(vec);
    return (1);
}

