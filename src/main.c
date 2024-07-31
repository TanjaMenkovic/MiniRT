#include "../headers/minirt.h"

static void check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        error_msg("number of arguments is not valid!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
    if (ft_strchr(argv[1], '.') == NULL || ft_strcmp(ft_strchr(argv[1], '.'), ".rt") != 0)
    {
        error_msg("argument is not correct!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    t_rt    *rt;
    int fd;
    t_index j;

    check_args(argc, argv);
    fd = open_file(argv[1]);
    init_index(&j);
    rt = (t_rt *)malloc(sizeof(t_rt));
    if (parse_rt(rt, fd, &j) == 0)
    {
        //free everything
        close(fd);
        return (1);
    }
    if (!rt->a.id)
    close(fd);
    return (0);
}