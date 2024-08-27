/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:40:46 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 13:54:37 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "vector.h"

/* in order to prevent bouncing rays self-intersection */
# define RAY_MIN 0.0001f

/* 'infinite' distance, used as a default value */
# define RAY_MAX 1.0e30f

typedef struct s_ray
{
	t_vector	start;
	t_vector	direction;
	float		t;
}	t_ray;

/* 
	HOLDS DATA FOR CLOSEST HIT OBJECT

	point = point where ray hits the object
	normal = the normal of the hit object
	color = color of object
	center = centerpoint for spheres and cylinders or a point that's inside a plane
	shape = 0 for sphere, 1 for plane, 2 for cylinder
	id = which position in geometrical object array
	t = distance from ray origin
*/
typedef struct s_hit_record
{
	t_vector	point;
	t_vector	color;
	t_vector	normal;
	t_vector	center;
	int			shape;
	int			id;
	float		t;

}	t_hit_record;

t_ray		init_ray(t_vector s, t_vector d);
t_vector	ray_point(t_ray r, float t);

#endif