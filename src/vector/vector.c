#include "../../headers/vector.h"

float	ft_sqr(float n)
{
	return (n * n);
}

float	vec_len(t_vector v)
{
	float	l;

	l = sqrt(v.x * v.x + v.y * v.y + v.z + v.z);
	return (l);
}

t_vector	vec_add(t_vector v, t_vector u)
{
	t_vector	w;
	w.x = u.x + v.x;
	w.y = u.y + v.y;
	w.z = u.z + v.z;
	return (w);
}

t_vector	vec_neg(t_vector v)
{
	t_vector	w;

	w.x = -v.x;
	w.y = -v.y;
	w.z = -v.z;
	return (w);
}

t_vector	vec_mult(t_vector v, float a)
{
	t_vector	w;

	w.x = a * v.x;
	w.y = a * v.y;
	w.z = a * v.z;
	return (w);
}
