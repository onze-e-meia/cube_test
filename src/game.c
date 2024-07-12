/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   game.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: tforster <tfforster@student.42sp.org.br	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/07/11 14:08:14 by tforster		  #+#	#+#			 */
/*   Updated: 2024/07/11 19:18:35 by tforster		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

#include "stuff.h"

#define WIDTH 512
#define HEIGHT 512

// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}



void ft_randomize(void* param)
{
	t_cube	*cube;

	cube = (t_cube *) param;

	t_vec3	v0 = {5, 5, 0};
	t_vec3	v1 = {100, 100, 0};

	memset(cube->img->pixels, 27, cube->img->width * cube->img->height * 4);
	bresenham(cube, &v0, &v1);

	t_mtx	mtx_z, mtx_x;
	float	theta = 45;
	rot_z(theta, &mtx_z);
	rot_x(theta, &mtx_x);

	int	i = 0;

	while (i < cube->mesh->nbr)
	{
		t_tri	tri_proj, tri_trans, tri_z, tri_zx;
		// Rotate in Z-Axis
		mult_mtx_vec(&cube->mesh->tris[i].vecs[0], &tri_z.vecs[0], &mtx_z);
		mult_mtx_vec(&cube->mesh->tris[i].vecs[1], &tri_z.vecs[1], &mtx_z);
		mult_mtx_vec(&cube->mesh->tris[i].vecs[2], &tri_z.vecs[2], &mtx_z);
		// Rotate in X-Axis
		mult_mtx_vec(&tri_z.vecs[0], &tri_zx.vecs[0], &mtx_x);
		mult_mtx_vec(&tri_z.vecs[1], &tri_zx.vecs[1], &mtx_x);
		mult_mtx_vec(&tri_z.vecs[2], &tri_zx.vecs[2], &mtx_x);
		// Offset into the screen
		tri_trans = tri_zx;
		tri_trans.vecs[0].z = tri_zx.vecs[0].z + 3.0;
		tri_trans.vecs[1].z = tri_zx.vecs[1].z + 3.0;
		tri_trans.vecs[2].z = tri_zx.vecs[2].z + 3.0;
		// Project triangles from 3D --> 2D
		mult_mtx_vec(&tri_trans.vecs[0], &tri_proj.vecs[0], cube->mtx);
		mult_mtx_vec(&tri_trans.vecs[1], &tri_proj.vecs[1], cube->mtx);
		mult_mtx_vec(&tri_trans.vecs[2], &tri_proj.vecs[2], cube->mtx);
		// Scale into view
		t_vec2	vec2;
		scale(&tri_proj, cube, &vec2);
		draw_tri(cube, &cube->mesh->tris[i]);
		i++;
	}

	// for (uint32_t i = 0; i < cube->img->width; ++i)
	// {
	// 	for (uint32_t y = 0; y < cube->img->height; ++y)
	// 	{
	// 		uint32_t color = ft_pixel(
	// 			rand() % 0xFF, // R
	// 			rand() % 0xFF, // G
	// 			rand() % 0xFF, // B
	// 			rand() % 0xFF  // A
	// 		);
	// 		mlx_put_pixel(cube->img, i, y, color);
	// 	}
	// }
	// // Clear Screan
	// memset(cube->img->pixels, 27, cube->img->width * cube->img->height * 4);
}

void ft_hook(void* param)
{
	t_cube	*cube;

	cube = (t_cube *) param;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(cube->mlx);
	if (mlx_is_key_down(cube->mlx, MLX_KEY_UP))
		cube->img->instances[0].y -= 5;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_DOWN))
		cube->img->instances[0].y += 5;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_LEFT))
		cube->img->instances[0].x -= 5;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_RIGHT))
		cube->img->instances[0].x += 5;
}

// -----------------------------------------------------------------------------

int32_t main(void)
{
	t_cube	cube;
	int		width = 256;
	int		height = 256;

	// mlx_t* mlx;

	// Gotta error check this stuff
	if (!(cube.mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(cube.img = mlx_new_image(cube.mlx, 256, 256)))
	{
		mlx_close_window(cube.mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(cube.mlx, cube.img, 100, 100) == -1)
	{
		mlx_close_window(cube.mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}

	t_tri tris[12] = {
		// SOUTH
		{{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} }},
		{{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }},
		// EAST
		{{ {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} }},
		{{ {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f} }},
		// NORTH
		{{ {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f} }},
		{{ {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }},
		// WEST
		{{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} }},
		{{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} }},
		// TOP
		{{ {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} }},
		{{ {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} }},
		// BOTTOM
		{{ {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} }},
		{{ {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }},
		};
	t_mesh	mesh;
	mesh.nbr = sizeof(tris) / sizeof(t_tri);
	mesh.tris = malloc(mesh.nbr * sizeof(t_tri));
	size_t	index = 0;
	while (index < mesh.nbr)
	{
		mesh.tris[index] = tris[index];
		index++;
	}

	t_mtx	proj_mtx;
	project_mtx(height, width, &proj_mtx);
	cube.mtx = &proj_mtx;

	printf("[%f][%f][%f]\n", mesh.tris[1].vecs[0].x, mesh.tris[1].vecs[1].y,mesh.tris[1].vecs[2].z);
	printf("MESH nbr [%zu]\n", mesh.nbr);
	cube.mesh = &mesh;

	mlx_loop_hook(cube.mlx, ft_randomize, &cube);
	mlx_loop_hook(cube.mlx, ft_hook, &cube);

	mlx_loop(cube.mlx);

	free(mesh.tris);
	mlx_terminate(cube.mlx);
	return (EXIT_SUCCESS);
}
