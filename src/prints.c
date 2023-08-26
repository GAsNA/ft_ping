#include "ft_ping.h"

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

