#ifndef RAY_H
# define RAY_H

# include "vector.h"

typedef struct s_ray
{
	t_vector start;
	t_vector direction;
	float t;
}	t_ray;

#endif