/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tforster <tfforster@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:18:28 by tforster          #+#    #+#             */
/*   Updated: 2024/07/12 19:26:54 by tforster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STUFF_H
# define STUFF_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <MLX42/MLX42.h>
# include <math.h>

typedef struct	s_vec2
{
	float		x;
	float		y;
}				t_vec2;

typedef struct	s_ivec2
{
	int		x;
	int		y;
}				t_ivec2;

typedef struct	s_vec3
{
	float		x;
	float		y;
	float		z;
}				t_vec3;

typedef struct	s_vec4
{
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vec4;

typedef struct	s_tri
{
	t_vec3		vecs[3];
}				t_tri;

typedef struct	s_mesh
{
	t_tri		*tris;
	size_t		nbr;
}				t_mesh;

typedef struct	s_mtx
{
	float m[4][4];
}				t_mtx;

typedef struct	s_cube
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_mesh		*mesh;
	t_mtx		*mtx;
}				t_cube;

void	project_mtx(int height, int width, t_mtx *proj_mtx);
void	mult_mtx_vec(t_vec3 *i, t_vec3 *o, t_mtx *m);
void	rot_z(float theta, t_mtx *mtx);
void	rot_x(float theta, t_mtx *mtx);
void	offset(t_tri *old, t_tri *new);
void	scale(t_tri *tri_proj, t_cube *cube, t_vec2 *vec2);

void	bresenham(t_cube *fdf, t_vec3 *v0, t_vec3 *v1);
void	draw_tri(t_cube *cube, t_tri *tri);

#endif
