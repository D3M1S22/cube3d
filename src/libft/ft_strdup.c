#include "../cud3d.h"

char	*ft_strdup(const char *s1)
{
	unsigned int		i;
	char				*ret;

	i = 0;
	while (s1[i])
		i++;
	ret = malloc(sizeof(char) * (i + 1));
	if (!ret)
		return ((void *)0);
	ret[i++] = '\0';
	i--;
	while (i--)
		ret[i] = s1[i];
	return (ret);
}