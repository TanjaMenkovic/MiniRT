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

    rt = NULL;
    check_args(argc, argv);
    printf("1\n");
    fd = open_file(argv[1]);
    printf("2\n");
    init_rt(rt, &j, argv);
    printf("%d %d %d\n", rt->num_cy, rt->num_pl, rt->num_sp);
    printf("3\n");
    if (parse_rt(rt, fd, &j) == 0)
    {
        printf("4\n");
        //free everything
        close(fd);
        return (1);
    }
    printf("5\n");
    // if (!rt->a.id)
    close(fd);
    return (0);
}