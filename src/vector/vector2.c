#include "../../headers/vector.h"

t_vector    unit_vector(t_vector v)
{
    return (vec_div(v, vec_len(v)));
}