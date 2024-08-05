#include "../../headers/vector.h"

t_vector	vec_mult(t_vector v, float a)
{
	t_vector	w;

	w.x = a * v.x;
	w.y = a * v.y;
	w.z = a * v.z;
	return (w);
}

t_vector	vec_div(t_vector v, float a)
{
	t_vector	w;

	if (a == 0)
	{
		printf("Can't divide with neg number!\n");
		exit(1);
	}
	w.x = v.x / a;
	w.y = v.y / a;
	w.z = v.z / a;
	return (w);
}

float	dot_prod(t_vector v, t_vector u)
{
	return (v.x * u.x + v.y * u.y + v.z * u.z);
}

t_vector	cross_prod(t_vector v, t_vector u)
{
	t_vector	w;

	w.x = v.y * u.z - v.z * u.y;
	w.y = v.z * u.x - v.x * u.z;
	w.z = v.x * u.y - v.y * u.x;
	return (w);
}

t_vector	vec_norm(t_vector v)
{
	t_vector	w;

	w = vec_div(v, vec_len(v));
	return (w);
}
