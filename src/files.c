#include "../headers/minirt.h"

static int	is_rt_file(char *file)
{
	int	len;

	len = ft_strlen(file) - 3;
	if (len > 3)
		return (ft_strncmp(file + len, ".rt", 3) == 0);
	return (0);
}

int open_file(char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        error_msg("file doesn't exist!\n", 0);
        exit(EXIT_FAILURE);
    }
    if (is_rt_file(file) == 0)
    if (fd < 0)
    {
        error_msg("not rt file!\n", 0);
        exit(EXIT_FAILURE);
    }
    return (fd);
}