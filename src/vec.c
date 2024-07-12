/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tforster <tfforster@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:34:18 by tforster          #+#    #+#             */
/*   Updated: 2024/07/12 19:24:28 by tforster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stuff.h"

// Function to initialize a mesh
void init_mesh(t_mesh* mesh)
{
	mesh->tris = NULL;
	// mesh->nbr = 0;
}

// Function to add a triangle to the mesh
// void add_triangle(t_mesh* mesh, t_tri tri)
// {	float		w;

// 	// Reallocate memory for one more triangle
// 	t_tri* new_tris = realloc(mesh->tris, (mesh->nbr + 1) * sizeof(t_tri));
// 	if (new_tris != NULL)
// 	{
// 		mesh->tris = new_tris;
// 		mesh->tris[mesh->nbr] = tri;
// 		mesh->nbr++;
// 	}
// }

// Function to free the memory used by the mesh
// void free_mesh(t_mesh* mesh)
// {
// 	free(mesh->tris);
// 	mesh->tris = NULL;
// 	mesh->nbr = 0;
// }

// int main()
// {
// 	t_mesh mesh;
// 	init_mesh(&mesh);

// 	t_tri tri = {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}};
// 	add_triangle(&mesh, tri);

// 	// Free memory used by the mesh
// 	free_mesh(&mesh);

// 	return 0;
// }

void	project_mtx(int height, int width, t_mtx *proj_mtx)
{
	float	near = 0.1f;
	float	far = 1000.0f;
	float	fov = 90.0f;
	float	aspct_rt = (float) height / (float) width;
	float	fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

	proj_mtx->m[0][0] = aspct_rt * fov_rad;
	proj_mtx->m[1][1] = fov_rad;
	proj_mtx->m[2][2] = far / (far - near);
	proj_mtx->m[3][2] = (-far * near) / (far - near);
	proj_mtx->m[2][3] = 1.0f;
	proj_mtx->m[3][3] = 0.0f;
}

void	mult_mtx_vec(t_vec3 *i, t_vec3 *o, t_mtx *m)
	{
		o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
		o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
		o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
		float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

		if (w != 0.0f)
		{
			o->x /= w; o->y /= w; o->z /= w;
		}
	}

void	rot_z(float theta, t_mtx *mtx)
{
	mtx->m[0][0] = cosf(theta);
	mtx->m[0][1] = sinf(theta);
	mtx->m[1][0] = -sinf(theta);
	mtx->m[1][1] = cosf(theta);
	mtx->m[2][2] = 1;
	mtx->m[3][3] = 1;
}

void	rot_x(float theta, t_mtx *mtx)
{
	mtx->m[0][0] = 1;
	mtx->m[1][1] = cosf(theta * 0.5f);
	mtx->m[1][2] = sinf(theta * 0.5f);
	mtx->m[2][1] = -sinf(theta * 0.5f);
	mtx->m[2][2] = cosf(theta * 0.5f);
	mtx->m[3][3] = 1;
}

void	offset(t_tri *old, t_tri *new)
{
	new = old;
	new->vecs[0].z = old->vecs[0].z + 3.0f;
	new->vecs[1].z = old->vecs[1].z + 3.0f;
	new->vecs[2].z = old->vecs[2].z + 3.0f;
}

void	scale(t_tri *tri_proj, t_cube *cube, t_vec2 *vec2)
{
	tri_proj->vecs[0].x += 1.0f; tri_proj->vecs[0].y += 1.0f;
	tri_proj->vecs[1].x += 1.0f; tri_proj->vecs[1].y += 1.0f;
	tri_proj->vecs[2].x += 1.0f; tri_proj->vecs[2].y += 1.0f;
	tri_proj->vecs[0].x *= 0.5f * (float) cube->img->width;
	tri_proj->vecs[0].y *= 0.5f * (float) cube->img->height;
	tri_proj->vecs[1].x *= 0.5f * (float) cube->img->width;
	tri_proj->vecs[1].y *= 0.5f * (float) cube->img->height;
	tri_proj->vecs[2].x *= 0.5f * (float) cube->img->width;
	tri_proj->vecs[2].y *= 0.5f * (float) cube->img->height;
}

void draw_tri(t_cube *cube, t_tri *tri)
{
	bresenham(cube, &tri->vecs[0], &tri->vecs[1]);
	bresenham(cube, &tri->vecs[1], &tri->vecs[2]);
	bresenham(cube, &tri->vecs[2], &tri->vecs[0]);

}
