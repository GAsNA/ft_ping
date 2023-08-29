#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>
# include <netinet/ip_icmp.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <errno.h>

# include "../libft/libft.h"

typedef struct s_ft_ping	t_ft_ping;
typedef struct s_ping_list	t_ping_list;

struct s_ft_ping
{
	int				verbose;
	struct addrinfo	*addrinfo;
	int				socket_fd;
	char			*addr;
	char			ip[INET_ADDRSTRLEN];
	int				id;
	int				sent;
	int				received;
	t_ping_list		*list;
	t_ping_list		*save;
};

struct s_ping_list
{
	int				sequence;
	struct timeval	time;
	double			diff;
	t_ping_list		*next;
};

typedef struct
{
	struct iphdr	ip;
	struct icmphdr	icmp;
	char			*data;
}	t_recv;

extern t_ft_ping	g_ping;

// MAIN.C
void		get_stats_time(double *min, double *max, double *avg, double *mdev);
void		stop(int sig);
void		clear_all(void);
void		init(int ac, char **av);
void		send_packet(struct icmphdr *icmp, struct timeval begin);
void		receive_packet(void);

// UTILS.C
unsigned short	calculate_icmp_checksum(unsigned short *buf, size_t length);
void			add_to_list(t_ping_list **list, t_ping_list *new);
void			clear_list(t_ping_list *list);
double			sqrt(double x);
double			pow(double x, double y);

// PRINTS.C
void		no_destination_address(void);
void		help(void);
void		try(void);
void		unknown_argument(char *arg);
void		unknown_name_service();
void		first_information(void);
void		last_information(void);

#endif
