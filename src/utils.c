#include "../headers/utils.h"

/* size of double string */
int doublelen(char **str)
{
    int n;

    n = 0;
    while (str[n] != 0)
        n++;
    return (n);
}

/* checking is number of elements correct */
void    numofel(char **str, int n)
{
    if (doublelen(str) != n)
    {
        error_msg("informations about element is not correct!\n", 0);
        exit(EXIT_FAILURE);
    }
}