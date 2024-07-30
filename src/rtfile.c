#include "../headers/minirt.h"

/* 
we need to see how actually this .rt file looks
and can it have also some random lines in it, or everything 
needs to be properly written.  
also can we have empty lines or not and so on.
we should use getnextline to read each line and based on line 
parse it.
we need to check if we need another split for first splitting it
based on white spaces, or it can be splited only based on space.
*/

/*
parsing functions are returning 1 in case nothing fails, and 0 if 
there are some errors
*/

int    parse_rt(char *file, t_rt *rt)
{
    int     fd;
    char    **line;
    char    *str;
    int     i;
    
    fd = open_file(file);
    
    //rt = malloc(sizeof(t_rt));
    while (fd)
    {
        i = 1;
        str = get_next_line(fd);
        line = ft_splitset(str, " \n\t\r\v\f");
        free(str);
        if (ft_strcmp(line[0], "A") == 0)
            i = parse_amb(line, rt);
        else if (ft_strcmp(line[0], "C") == 0)
            i = parse_amb(line, rt);
        else if (ft_strcmp(line[0], "L") == 0)
            i = parse_amb(line, rt);
        else if (ft_strcmp(line[0], "sp") == 0)
            i = parse_amb(line, rt);
        else if (ft_strcmp(line[0], "pl") == 0)
            i = parse_amb(line, rt);
        else if (ft_strcmp(line[0], "cy") == 0)
            i = parse_amb(line, rt);
        else if (line[0])
        {
            i = 0;
            error_msg("incorrect value in .rt file!\n", "Correct starting with: A/C/L/sp/pl/cy");
        }
        if (i == 0)
        {
            free_arr(line);
            close (fd);
            return (0);
        }
        free_arr(line);
    }

    close(fd);
    return (1);
}
