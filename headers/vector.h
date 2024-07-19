#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

# include <unistd.h>
# include <stdio.h>
# include "../libft/headers/libft.h"

typedef struct s_vector
{
	float x;
	float y;
	float z;
}	t_vector;

typedef struct	s_point
{
	float x;
	float y;
}	t_point;

//math functions
float		ft_sqr(float n);

//vector functions
float		vec_len(t_vector v);
t_vector	vec_add(t_vector v, t_vector u);
t_vector	vec_neg(t_vector v);
t_vector	vec_mult(t_vector v, float a);
t_vector	vec_div(t_vector v, float a);
float		dot_prod(t_vector v, t_vector u);
t_vector	cross_prod(t_vector v, t_vector u);
t_vector	vec_norm(t_vector v);

#endif
