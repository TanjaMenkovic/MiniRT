#include "../headers/minirt.h"

void	set_px_col(mlx_image_t *img, int x, int y, unsigned int color)
{
	int	index;

	index = (y * (int)img->width + x) * 4;
	if (x < (int)img->width && y < (int)img->height)
	{
		img->pixels[index] = (color >> 24) & 0xFF;
		img->pixels[index + 1] = (color >> 16) & 0xFF;
		img->pixels[index + 2] = (color >> 8) & 0xFF;
		img->pixels[index + 3] = color & 0xFF;
	}
}

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}
