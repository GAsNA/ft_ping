#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../libft/libft.h"

void	no_destination_address(void)
{
	printf("ft_ping: usage error: Destination address required\n");
}

void	help(void)
{
	printf("\nUsage\n");
	printf("\tft_ping [options] <destination>\n");
	printf("\n");
	printf("Options:\n");
	printf("\t<destination>\tdns name or ip address\n");
	printf("\t-h\t\tprint help and exit\n");
	printf("\t-v\t\tverbose output\n");
}

void	unknown_argument(char *arg)
{
	printf("ft_ping: invalid argument: \"%s\"\n", arg);
}

void	unknown_name_service(char *name)
{
	printf("ft_ping: %s: Name or service not known\n", name);
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		no_destination_address();
		return 1;
	}

	int	verbose = 0; (void)verbose;
	char *addr;

	for(int i = 1; i < ac; i++)
	{
		if (!ft_strcmp(av[i], "-h")) { help(); return 0; }
		else if (!ft_strcmp(av[i], "-v")) { verbose = 1; }
		else if (av[i][0] == '-') { unknown_argument(av[i]); help(); return 1; }
		else { addr = av[i]; }
	}

	if (!addr) { no_destination_address(); return 1; }

	struct addrinfo	*result;
	if (getaddrinfo(addr, NULL, NULL, &result)) { unknown_name_service(addr); return 1; }

	return 0;
}
