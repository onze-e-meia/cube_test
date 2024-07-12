/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tforster <tfforster@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 13:58:08 by tforster          #+#    #+#             */
/*   Updated: 2024/07/12 19:24:59 by tforster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stuff.h"

typedef struct s_bresenham
{
	int		dx;
	int		sx;
	int		dy;
	int		sy;
	int		err;
	int		e2;
	float	dist;
	float	step;
}			t_ctrl;

static void	init_ctrl_pts(t_ctrl *ctrl, t_ivec2 *v0, t_ivec2 *v1);
static void	draw_line(t_cube *fdf, t_ivec2 *v0, t_ivec2 *v1, t_ctrl *ctrl);
static void	point_in_vp(t_cube *fdf, t_ivec2 *start, t_ivec2 *end, t_ctrl *ctrl);

float	dist_2d(t_vec3 *v0, t_vec3 *v1)
{
	return (sqrtf(powf(v1->x - v0->x, 2) + powf(v1->y - v0->y, 2)));
}

void	bresenham(t_cube *fdf, t_vec3 *v0, t_vec3 *v1)
{
	t_ctrl	ctrl;
	t_ivec2	iv0;
	t_ivec2	iv1;

	iv0.x = (uint32_t) v0->x;
	iv0.y = (uint32_t) v0->y;
	iv1.x = (uint32_t) v1->x;
	iv1.y = (uint32_t) v1->y;

	init_ctrl_pts(&ctrl, &iv0, &iv1);
	ctrl.dist = (float) 1 / dist_2d(v0, v1);
	draw_line(fdf, &iv0, &iv1, &ctrl);
}

static void	init_ctrl_pts(t_ctrl *ctrl, t_ivec2 *v0, t_ivec2 *v1)
{
	ctrl->dx = abs(v1->x - v0->x);
	ctrl->sx = (1 * (v0->x < v1->x) - 1 * (v0->x >= v1->x));
	ctrl->dy = -abs(v1->y - v0->y);
	ctrl->sy = (1 * (v0->y < v1->y) - 1 * (v0->y >= v1->y));
	ctrl->err = ctrl->dx + ctrl->dy;
	ctrl->dist = 0;
	ctrl->step = 0;
}

static void	draw_line(t_cube *fdf, t_ivec2 *v0, t_ivec2 *v1, t_ctrl *ctrl)
{
	while (1)
	{
		point_in_vp(fdf, v0, v1, ctrl);
		if (v0->x == v1->x
			&& v0->y == v1->y)
			break ;
		ctrl->e2 = 2 * ctrl->err;
		if (ctrl->e2 >= ctrl->dy)
		{
			if (v0->x == v1->x)
				break ;
			ctrl->err += ctrl->dy;
			v0->x += ctrl->sx;
		}
		if (ctrl->e2 <= ctrl->dx)
		{
			if (v0->y == v1->y)
				break ;
			ctrl->err += ctrl->dx;
			v0->y += ctrl->sy;
		}
	}
}

static void	point_in_vp(t_cube *fdf, t_ivec2 *start, t_ivec2 *end, t_ctrl *ctrl)
{
	// t_color		color;

	if ((3 <= start->x && start->x < fdf->img->width - 3)
		&& (3 <= start->y && start->y < fdf->img->height - 3))
	{
		// color = intrplt_color(start->color, end->color, ctrl->step);
		ctrl->step += (ctrl->step <= 1) * ctrl->dist + (ctrl->step > 1) * 1;
		mlx_put_pixel(fdf->img, start->x, start->y, 0xFF0000FF);
	}
}
