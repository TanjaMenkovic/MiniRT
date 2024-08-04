#include "../headers/libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1)
		return 1;
	i = 0;
	if (!s1 || !s2)
        return (1);
	while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
