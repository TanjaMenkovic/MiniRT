#include "../headers/minirt.h"

int open_file(char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        error_msg("file can't be opened!\n", 0);
        exit(EXIT_FAILURE);
    }
    return (fd);
}