/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:49:52 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/08/28 00:12:59 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

float	hit_top_cap(t_vec axis_dir, t_vec top_cap_c, t_ray ray, float r)
{
	float		t_cap_top;
	t_vec		point_top;
	t_vec		vec_top;

	t_cap_top = hit_plane(axis_dir, top_cap_c, ray);
	if (t_cap_top > 0.0f)
	{
		point_top = vec_add(ray.start, vec_mult(ray.direction, t_cap_top));
		vec_top = vec_sub(point_top, top_cap_c);
		if (dot_prod(vec_top, vec_top) > r * r)
			t_cap_top = -1.0f;
	}
	return (t_cap_top);
}

float	hit_bot_cap(t_vec axis_dir, t_vec bot_cap_c, t_ray ray, float r)
{
	float		t_cap_bot;
	t_vec		point_bot;
	t_vec		vec_bot;

	t_cap_bot = hit_plane(axis_dir, bot_cap_c, ray);
	if (t_cap_bot > 0.0f)
	{
		point_bot = vec_add(ray.start, vec_mult(ray.direction, t_cap_bot));
		vec_bot = vec_sub(point_bot, bot_cap_c);
		if (dot_prod(vec_bot, vec_bot) > r * r)
			t_cap_bot = -1.0f;
	}
	return (t_cap_bot);
}

float	hit_cyl_body(t_ray ray, t_cylinder c, float r, float h)
{
	t_cyl_body	cy;

	cy.oc = vec_sub(ray.start, c.center);
	cy.oc_perp = vec_sub(cy.oc, vec_mult(c.axis, dot_prod(cy.oc, c.axis)));
	cy.ray_dir_perp = vec_sub(ray.direction,
			vec_mult(c.axis, dot_prod(ray.direction, c.axis)));
	cy.a = dot_prod(cy.ray_dir_perp, cy.ray_dir_perp);
	cy.b = 2.0f * dot_prod(cy.ray_dir_perp, cy.oc_perp);
	cy.c = dot_prod(cy.oc_perp, cy.oc_perp) - r * r;
	cy.discriminant = cy.b * cy.b - 4.0f * cy.a * cy.c;
	if (cy.discriminant < 0.0f)
		return (-1.0f);
	cy.sqrt_discriminant = sqrtf(cy.discriminant);
	cy.t1 = (-cy.b - cy.sqrt_discriminant) / (2.0f * cy.a);
	cy.t2 = (-cy.b + cy.sqrt_discriminant) / (2.0f * cy.a);
	cy.t_cylinder = -1.0f;
	cy.p1 = vec_add(ray.start, vec_mult(ray.direction, cy.t1));
	cy.p2 = vec_add(ray.start, vec_mult(ray.direction, cy.t2));
	cy.z1 = dot_prod(vec_sub(cy.p1, c.center), c.axis);
	cy.z2 = dot_prod(vec_sub(cy.p2, c.center), c.axis);
	if (cy.t1 > 0 && cy.z1 >= -h / 2.0f && cy.z1 <= h / 2.0f)
		cy.t_cylinder = cy.t1;
	else if (cy.t2 > 0 && cy.z2 >= -h / 2.0f && cy.z2 <= h / 2.0f)
		cy.t_cylinder = cy.t2;
	return (cy.t_cylinder);
}

float	hit_cylinder(t_cylinder c, t_ray ray)
{
	float		t_cylinder;
	float		t_cap_top;
	float		t_cap_bottom;
	t_vec		top_cap_center;
	t_vec		bot_cap_center;

	c.axis = unit_vec(c.axis);
	bot_cap_center = vec_sub(c.center, vec_mult(c.axis, c.height / 2.0f));
	top_cap_center = vec_add(c.center, vec_mult(c.axis, c.height / 2.0f));
	t_cylinder = hit_cyl_body(ray, c, c.radius, c.height);
	t_cap_top = hit_top_cap(c.axis, top_cap_center, ray, c.radius);
	t_cap_bottom = hit_bot_cap(c.axis, bot_cap_center, ray, c.radius);
	if (t_cylinder < 0.0f || (t_cap_top > 0.0f && t_cap_top < t_cylinder))
		t_cylinder = t_cap_top;
	if (t_cylinder < 0.0f || (t_cap_bottom > 0.0f && t_cap_bottom < t_cylinder))
		t_cylinder = t_cap_bottom;
	return (t_cylinder);
}

// t_vec	cyl_normal(t_vec point, t_cylinder c, float height, float radius)
// {
// 	t_cyl_normal	cy;

// 	c.axis = unit_vec(c.axis);
// 	cy.base_center = vec_sub(c.center, vec_mult(c.axis, height / 2.0f));
// 	cy.top_center = vec_add(c.center, vec_mult(c.axis, height / 2.0f));
// 	cy.p_minus_c = vec_sub(point, c.center);
// 	cy.proj_length = dot_prod(cy.p_minus_c, c.axis);
// 	if (cy.proj_length >= height / 2.0f)
// 		return (c.axis);
// 	else if (cy.proj_length <= -height / 2.0f)
// 		return (vec_mult(c.axis, -1.0f));
// 	cy.proj_p_minus_c = vec_mult(c.axis, cy.proj_length);
// 	cy.normal = vec_sub(cy.p_minus_c, cy.proj_p_minus_c);
// 	cy.normal = unit_vec(cy.normal);
// 	return (cy.normal);
// }

t_vec cyl_normal(t_vec point, t_cylinder c, float height, float radius) {
	c.axis = unit_vec(c.axis);
    // Vector from the cylinder center to the point
    t_vec center_to_point = vec_sub(point, c.center);

    // Project the vector onto the cylinder axis to get the height component
    float proj_length = dot_prod(center_to_point, c.axis);
    
    // Clamp the projection length to the height of the cylinder
    if (proj_length < 0.0f) proj_length = 0.0f;
    if (proj_length > height) proj_length = height;

    // Find the closest point on the cylinder axis to the point on the surface
    t_vec closest_point_on_axis = vec_add(c.center, vec_mult(c.axis, proj_length));
    
    // Vector from the closest point on the axis to the point on the surface
    t_vec normal = vec_sub(point, closest_point_on_axis);
    
    // If the magnitude of this vector is approximately equal to the radius, it's on the curved surface
    float distance = sqrt(dot_prod(normal, normal));
    if (fabs(distance - radius) < 1e-6) {
        return unit_vec(normal);
    }
    
    // If the point is not on the curved surface, it could be on one of the caps
    // Check if the point is on the top or bottom cap by comparing with the axis direction
    if (proj_length <= 1e-6) {  // Point is on the bottom cap
        return vec_mult(c.axis, -1.0f);
    } else if (fabs(proj_length - height) <= 1e-6) {  // Point is on the top cap
        return c.axis;
    }

    // Default case (should not happen for valid input)
    t_vec invalid_normal = {0.0f, 0.0f, 0.0f};
    return invalid_normal;
}