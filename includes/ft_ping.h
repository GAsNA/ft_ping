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

// MAIN.C
void		get_stats_time(double *min, double *max, double *avg, double *mdev);
void		stop(int sig);

// UTILS.C
uint16_t	calculate_icmp_checksum(void *data, size_t length);
void		add_to_list(t_ping_list **list, t_ping_list *new);
void		clear_list(t_ping_list *list);
double		sqrt(double x);
double		pow(double x, double y);

// PRINTS.C
void		no_destination_address(void);
void		help(void);
void		unknown_argument(char *arg);
void		unknown_name_service(char *name);

#endif
