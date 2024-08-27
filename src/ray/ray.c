/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:29:23 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 13:29:24 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ray.h"

t_ray	init_ray(t_vector s, t_vector d)
{
	t_ray	r;

	r.start = s;
	r.direction = d;
	r.t = RAY_MAX;
	return (r);
}

t_vector	ray_point(t_ray r, float t)
{
	return (vec_add(r.start, vec_mult(r.direction, t)));
}
