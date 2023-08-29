# include "ft_ping.h"

t_ft_ping	g_ping;

void	get_stats_time(double *min, double *max, double *avg, double *mdev)
{
	t_ping_list	*tmp = g_ping.save;
	if (!tmp) { return; }

	*avg = tmp->diff;
	int		total = 1;

	*min = tmp->diff;
	*max = tmp->diff;
	tmp = tmp->next;

	// stddev
	// https://www.programiz.com/cpp-programming/examples/standard-deviation#google_vignette

	while (tmp)
	{
		if (*min > tmp->diff) { *min = tmp->diff; }
		if (*max < tmp->diff) { *max = tmp->diff; }
		*avg += tmp->diff;
		total++;
		tmp = tmp->next;
	}

	*avg /= total;

	tmp = g_ping.save;
	while (tmp) { *mdev += pow(tmp->diff - *avg, 2); tmp = tmp->next; }

	*mdev = sqrt(*mdev / total);
}

void	clear_all(void)
{
	close(g_ping.socket_fd);
	freeaddrinfo(g_ping.addrinfo);
	
	// CLEAR LIST
	clear_list(g_ping.list);
	clear_list(g_ping.save);
}

void	stop(int sig)
{
	(void)sig;
	last_information();
	clear_all();
	exit(0);
}

void	init(int ac, char **av)
{
	g_ping.verbose = 0;
	g_ping.sent = 0;
	g_ping.received = 0;

	for(int i = 1; i < ac; i++)
	{
		if (!ft_strcmp(av[i], "-?")) { help(); exit(0); }
		else if (!ft_strcmp(av[i], "-v")) { g_ping.verbose = 1; }
		else if (av[i][0] == '-') { unknown_argument(av[i]); exit(1); }
		else { g_ping.addr = av[i]; }
	}

	if (!g_ping.addr) { no_destination_address(); exit(1); }

	// GET INFO FROM ADDRESS
	if (getaddrinfo(g_ping.addr, NULL, NULL, &g_ping.addrinfo)) { unknown_name_service(); exit(1); }

	// SOCKET
	g_ping.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.socket_fd < 0) { printf("ft_ping: error: creating socket failed\n"); exit(1); }

	// SETSOCKOPT
	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if (setsockopt(g_ping.socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1) {
		printf("ft_ping: error: setsockopt failed\n"); exit(1); }

	// GET IP
	struct sockaddr_in *dest = (struct sockaddr_in *)g_ping.addrinfo->ai_addr;
	inet_ntop(AF_INET, &(dest->sin_addr), g_ping.ip, INET_ADDRSTRLEN);

	g_ping.id = getpid();
}

void	send_packet(struct icmphdr *icmp, struct timeval begin)
{
	icmp->un.echo.sequence++;
	icmp->checksum = 0;
	icmp->checksum = calculate_icmp_checksum((void*)icmp, sizeof(*icmp));
		
	// SEND PACKET
	ssize_t	res = sendto(g_ping.socket_fd, icmp, sizeof(*icmp), 0,
							g_ping.addrinfo->ai_addr, g_ping.addrinfo->ai_addrlen);
	if (res == sizeof(*icmp)) { g_ping.sent++; }

	// ADD TO LIST OF SENT PING
	t_ping_list	*new = malloc(sizeof(t_ping_list));
	if (!new) { printf("ft_ping: error: malloc failed."); clear_all(); exit(1); }
	new->sequence = icmp->un.echo.sequence;
	new->time = begin;
	new->next = NULL;
	add_to_list(&g_ping.list, new);
}

void	receive_packet(void)
{
	// RECEIVE PING
	t_recv			buf;
	struct iovec	iovec;
	iovec.iov_base = &buf; iovec.iov_len = sizeof(buf);
			
	struct msghdr	msg;
	msg.msg_name = NULL; msg.msg_namelen = 0;
	msg.msg_iov = &iovec; msg.msg_iovlen = 1;
	msg.msg_control = NULL; msg.msg_controllen = 0;
	//msg.msg_flags = 0;
	ssize_t	ret = recvmsg(g_ping.socket_fd, &msg, 0);
	if (ret == -1) {
		if (errno != EAGAIN) { printf("ft_ping: error: recvmsg failed.\t%s\n", strerror(errno)); clear_all(); exit(1); }
		else { return; }
	}

	if (buf.icmp.type != ICMP_ECHOREPLY) { return; }

	if (buf.icmp.un.echo.id != g_ping.id) { return; }

	g_ping.received++;

	struct timeval	now;
	gettimeofday(&now, NULL);

	// GET TIME OF THE RECEIVED PING
	t_ping_list	*received_ping = g_ping.list;
	while (received_ping)
	{
		if (received_ping->sequence == buf.icmp.un.echo.sequence) { break; }
		received_ping = received_ping->next;
	}
	struct timeval	time = received_ping->time;

	// CALCUL PING TIME
	double diff = (now.tv_sec - time.tv_sec) * 1000 + (double)(now.tv_usec - time.tv_usec) / 1000;
	received_ping->diff = diff;

	// PRINT INFORMATIONS ABOUT THIS PACKET
	printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", ret, g_ping.ip,
				buf.icmp.un.echo.sequence, buf.ip.ttl, diff);

	//MOVE THIS PACKET IN SAVE LIST
	t_ping_list	*tmp = g_ping.list;
	if (tmp->sequence == buf.icmp.un.echo.sequence)
	{
		g_ping.list = g_ping.list->next;
		tmp->next = NULL;
		add_to_list(&g_ping.save, tmp);
	}
	else
	{
		while (tmp->next)
		{
			if (tmp->next->sequence == buf.icmp.un.echo.sequence)
			{
				t_ping_list	*save = tmp->next;
				tmp->next = tmp->next->next;
				save->next = NULL;
				add_to_list(&g_ping.save, save);
				break;
			}
			tmp = tmp->next;
		}
	}
}

int	main(int ac, char **av)
{
	if (ac < 2) { no_destination_address(); return 1; }

	init(ac, av);

	// FIRST INFORMATION
	first_information();

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
	icmp.un.echo.sequence = -1;
	//icmp.un.gateway
	//icmp.un.frag.__unused
	//icmp.un.frag.mtu
	while (1)
	{
		gettimeofday(&begin, NULL);
		send_packet(&icmp, begin);

		// WAIT TILL TOTAL OF LOOP IS 1 SECOND
		do
		{
			receive_packet();
			gettimeofday(&end, NULL);
		} while (end.tv_sec - begin.tv_sec < 1);
	}

	return 0;
}
