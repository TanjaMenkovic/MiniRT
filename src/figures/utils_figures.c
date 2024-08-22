#include "../headers/minirt.h"

/*
    I = incoming vector
    N = normal vector, vector perpendicular to hit point
    I - 2.0 * dot(N, I) * N.
*/
t_vector reflect(t_vector I, t_vector N)
{
    t_vector norm;
    t_vector reflection;

    norm = unit_vector(N);
    reflection = vec_mult(norm, dot_prod(norm, I) * 2.0);
    reflection = vec_sub(I, reflection);
    return (reflection);
}