/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:40:57 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 14:02:27 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

# include <unistd.h>
# include <stdio.h>
# include "../libft/headers/libft.h"

typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vec;

typedef struct s_point
{
	float	x;
	float	y;
}	t_point;

/* vector.c */
float		ft_sqr(float n);
float		vec_len(t_vector v);
t_vector	vec_add(t_vector v, t_vector u);
t_vector	vec_sub(t_vector v, t_vector u);
t_vector	vec_neg(t_vector v);

/* vector1.c */
t_vector	vec_mult(t_vector v, float a);
t_vector	vec_div(t_vector v, float a);
float		dot_prod(t_vector v, t_vector u);
t_vector	cross_prod(t_vector v, t_vector u);
t_vector	vec_norm(t_vector v);

/* vector2.c */
t_vector	unit_vector(t_vector v);

#endif
