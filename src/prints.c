#include "ft_ping.h"

void	no_destination_address(void)
{
	printf("ft_ping: missing host operand\n");
	try();
}

void	help(void)
{
	printf("Usage: ft_ping [OPTION...] HOST...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	printf("\n");
	printf("\tOptions valid for all request types:\n");
	printf("\n");
	printf("\t\t-v\tverbose output\n");
	printf("\t\t-?\tgive this help list\n");
}

void	try(void)
{
	printf("Try 'ft_ping -?' for more information.\n");
}

void	unknown_argument(char *arg)
{
	printf("ft_ping: invalid option -- '%s'\n", arg);
	try();
}

void	unknown_name_service(void)
{
	printf("ft_ping: unknown host\n");
}

void	first_information(void)
{
	printf("FT_PING %s (%s): %lu data bytes", g_ping.addr, g_ping.ip, sizeof(struct icmphdr) + sizeof(struct iphdr));
	if (g_ping.verbose) { printf(", id 0x%x = %d", g_ping.id, g_ping.id); }
	printf("\n");
}

void	last_information(void)
{
	double	loss = g_ping.sent != 0 ? (g_ping.sent - g_ping.received) * 100 / g_ping.sent : 0;
	double	min = 0.0;
	double	max = 0.0;
	double	avg = 0.0;
	double	mdev = 0.0;

	get_stats_time(&min, &max, &avg, &mdev);

	printf("--- %s ping statistics ---\n", g_ping.addr);
	printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",
				g_ping.sent, g_ping.received, loss);
	if (loss == 0)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", min, avg, max, mdev);
}
