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

void	first_information(void)
{
	printf("FT_PING %s (%s): %d data bytes", g_ping.addr, g_ping.ip, 56); // TODO what is 56 ?
	if (g_ping.verbose) { printf(", id 0x%x = %d", g_ping.id, g_ping.id); }
	printf("\n");
}

void	last_information(void)
{
	double	loss = (g_ping.sent - g_ping.received) * 100 / g_ping.sent;
	double	min = 0.0;
	double	max = 0.0;
	double	avg = 0.0;
	double	mdev = 0.0;

	get_stats_time(&min, &max, &avg, &mdev);

	printf("\n--- %s ping statistics ---\n", g_ping.addr);
	printf("%d packets transmitted, %d packets received, %.0f%% packet loss, time [NB]ms\n",
				g_ping.sent, g_ping.received, loss);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", min, avg, max, mdev);
}
