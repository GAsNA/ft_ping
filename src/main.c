#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

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

struct addrinfo	*result;
int				socket_fd;

void	stop(int sig)
{
	(void)sig;
	close(socket_fd);
	freeaddrinfo(result);
	exit(0);
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		no_destination_address();
		return 1;
	}

	int	verbose = 0;
	char *addr;

	for(int i = 1; i < ac; i++)
	{
		if (!ft_strcmp(av[i], "-h")) { help(); return 0; }
		else if (!ft_strcmp(av[i], "-v")) { verbose = 1; }
		else if (av[i][0] == '-') { unknown_argument(av[i]); help(); return 1; }
		else { addr = av[i]; }
	}

	if (!addr) { no_destination_address(); return 1; }

	// GET INFO FROM ADDRESS
	if (getaddrinfo(addr, NULL, NULL, &result)) { unknown_name_service(addr); return 1; }

	// SOCKET
	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0) { printf("ft_ping: error: creating socket failed\n"); return 1; }

	// SETSOCKOPT
	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1) {
		printf("ft_ping: error: setsockopt failed\n"); return 1; }

	// GET IP
	struct sockaddr_in *dest = (struct sockaddr_in *)result->ai_addr;
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(dest->sin_addr), ip, INET_ADDRSTRLEN);

	int id = getpid();

	// FIRST INFORMATION
	printf("FT_PING %s (%s): %d data bytes", addr, ip, 56); // TODO what is 56 ?
	if (verbose)
		printf(", id 0x%x = %d", id, id);
	printf("\n");

	// HANDLING SIGNAL
	signal(SIGINT, (void *)&stop);

	int	nb = 0;
	while (1)
	{
		nb++;
		char buf[100] = "Hello";
		ssize_t	res = sendto(socket_fd, buf, sizeof(buf), 0, result->ai_addr, result->ai_addrlen);
		if (res == sizeof(buf)) { printf("SEND %d\n", nb); }
		sleep(1);
	}

	return 0;
}
