#include <stdio.h>

#include "../libft/libft.h"

void	help(void)
{
	printf("This is help...\n");
}

void	unknown_argument(void)
{
	printf("This is unknown argument...\n");
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		printf("ft_ping: usage error: Destination address required\n");
		return 1;
	}

	for(int i = 1; i < ac; i++)
	{
		if (!ft_strcmp(av[i], "-h")) { help(); return 0; }
		if (!ft_strcmp(av[i], "-v")) { /*verbose*/ }
		else if (av[i][0] == '-') { unknown_argument(); help(); return 1; }
		else { /*argument is to ping*/ }
	}

	return 0;
}
