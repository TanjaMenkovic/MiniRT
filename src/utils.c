#include "../headers/utils.h"

/* size of double string */
int doublelen(char **str)
{
    int n;

    n = 0;
    while (str[n])
        n++;
    return (n);
}

/* checking is number of elements correct */
void    numofel(char **str, int n)
{
    if (doublelen(str) != n)
    {
        error_msg("incorrect number of elements in a line!\n", 0);
        exit(EXIT_FAILURE);
    }
}

/* freeing array of strings */
void    free_arr(char **str)
{
    int i;
    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

/* atoi for float */
