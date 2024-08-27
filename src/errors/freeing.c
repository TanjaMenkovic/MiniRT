/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:26:25 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 17:42:32 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minirt.h"

void	free_pars(t_rt *rt)
{
	free(rt->sp);
	free(rt->pl);
	free(rt->cy);
}

/*
typedef struct s_rt
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	float		aspect_ratio;
	t_amb		a;
	t_camera	c;
	t_light		l;
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;
	int			width;
	int			height;
	int			num_sp;
	int			num_pl;
	int			num_cy;
}	t_rt;
*/

void	free_all(t_rt *rt)
{
	free_pars(rt);
	mlx_terminate(rt->mlx);
}
