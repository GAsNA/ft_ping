#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <sys/time.h>

#include "../libft/libft.h"

typedef struct s_ft_ping	t_ft_ping;
typedef struct s_ping_list	t_ping_list;

struct s_ft_ping
{
	struct addrinfo	*addrinfo;
	int				socket_fd;
	char			*addr;
	char			ip[INET_ADDRSTRLEN];
	int				id;
	t_ping_list		*list;
};

struct s_ping_list
{
	int				sequence;
	struct timeval	time;
	t_ping_list		*next;
};

t_ft_ping	g_ping;

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

void	stop(int sig)
{
	(void)sig;
	close(g_ping.socket_fd);
	freeaddrinfo(g_ping.addrinfo);
	
	// CLEAR LIST
	while (g_ping.list)
	{
		t_ping_list	*tmp = g_ping.list->next;
		free(g_ping.list);
		g_ping.list = tmp;
	}
	g_ping.list = NULL;

	exit(0);
}

uint16_t calculate_icmp_checksum(void *data, size_t length)
{
	const uint16_t	*buf = data;
	uint32_t		sum;

	sum = 0;
    while (length > 1) { sum += *buf++; length -= 2; }

	if (length > 0) { sum += *(uint8_t *)buf; }

	while (sum >> 16) { sum = (sum & 0xFFFF) + (sum >> 16); }

    return (uint16_t)~sum;
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		no_destination_address();
		return 1;
	}

	int	verbose = 0;

	for(int i = 1; i < ac; i++)
	{
		if (!ft_strcmp(av[i], "-h")) { help(); return 0; }
		else if (!ft_strcmp(av[i], "-v")) { verbose = 1; }
		else if (av[i][0] == '-') { unknown_argument(av[i]); help(); return 1; }
		else { g_ping.addr = av[i]; }
	}

	if (!g_ping.addr) { no_destination_address(); return 1; }

	// GET INFO FROM ADDRESS
	if (getaddrinfo(g_ping.addr, NULL, NULL, &g_ping.addrinfo)) { unknown_name_service(g_ping.addr); return 1; }

	// SOCKET
	g_ping.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.socket_fd < 0) { printf("ft_ping: error: creating socket failed\n"); return 1; }

	// SETSOCKOPT
	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if (setsockopt(g_ping.socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1) {
		printf("ft_ping: error: setsockopt failed\n"); return 1; }

	// GET IP
	struct sockaddr_in *dest = (struct sockaddr_in *)g_ping.addrinfo->ai_addr;
	inet_ntop(AF_INET, &(dest->sin_addr), g_ping.ip, INET_ADDRSTRLEN);

	g_ping.id = getpid();

	// FIRST INFORMATION
	printf("FT_PING %s (%s): %d data bytes", g_ping.addr, g_ping.ip, 56); // TODO what is 56 ?
	if (verbose) { printf(", id 0x%x = %d", g_ping.id, g_ping.id); }
	printf("\n");

	// HANDLING SIGNAL
	signal(SIGINT, (void *)&stop);

	// LOOP
	struct timeval	begin;
	struct timeval	end;

	struct icmphdr	icmp;
	icmp.type = ICMP_ECHO;
	icmp.code = 0;
	icmp.checksum = 0;
	icmp.un.echo.id = g_ping.id;
	icmp.un.echo.sequence = 0;
	//icmp.un.gateway
	//icmp.un.frag.__unused
	//icmp.un.frag.mtu
	while (1)
	{
		gettimeofday(&begin, NULL);
		
		icmp.checksum = calculate_icmp_checksum((void*)&icmp, sizeof(icmp));
		icmp.un.echo.sequence++;
		
		// SEND PACKET
		ssize_t	res = sendto(g_ping.socket_fd, &icmp, sizeof(icmp), 0, g_ping.addrinfo->ai_addr, g_ping.addrinfo->ai_addrlen);
		if (res == sizeof(icmp)) { printf("SEND %d\n", icmp.un.echo.sequence); }

		// ADD TO LIST OF SENT PING
		t_ping_list	*new = malloc(sizeof(t_ping_list));
		if (!new) { printf("ft_ping: error: malloc failed."); stop(0); exit(1); }
		new->sequence = icmp.un.echo.sequence;
		new->time = begin;
		new->next = NULL;
		t_ping_list	*tmp = g_ping.list;
		if (!g_ping.list) { g_ping.list = new; }
		else
		{
			while (tmp->next) { tmp = tmp->next; }
			tmp->next = new;
		}

		// WAIT TILL TOTAL OF LOOP IS 1 SECOND
		do
		{
			// RECEIVE PING
			struct msghdr	msg; //TODO WHAT IN MSG
			ssize_t	ret = recvmsg(g_ping.socket_fd, &msg, 0);
			if (ret == -1) { printf("ft_ping: error: recvmsg failed."); stop(0); exit(1); }

			//TODO IF ID IS DIFFERENT, CONTINUE

			struct timeval	now;
			gettimeofday(&now, NULL);

			// GET TIME OF THE RECEIVED PING
			struct timeval	time;
			tmp = g_ping.list;
			while (tmp)
			{
				if (tmp->sequence == icmp.un.echo.sequence) { time = tmp->time; }
				tmp = tmp->next;
			}

			// CALCUL PING TIME
			double diff = (now.tv_sec - time.tv_sec) * 1000 + (double)(now.tv_usec - time.tv_usec) / 1000;

			// PRINT INFORMATIONS ABOUT THIS PACKET
			printf("%zd bytes from [something] (%s): icmp_seq=%d ttl=%d time=%f ms\n", ret, g_ping.ip, 1, 1, diff);

			// REMOVE THIS PACKET
			/*tmp = g_ping.list;
			while (tmp->next)
			{
				if (tmp->next->sequence == sequence_buf)
				{
					t_ping_list	*tmp1 = tmp->next->next;
					free(tmp->next);
					tmp->next = tmp1;
				}
				tmp = tmp->next;
			}*/
	
			gettimeofday(&end, NULL);
		} while (end.tv_sec - begin.tv_sec < 1);
	}

	return 0;
}
