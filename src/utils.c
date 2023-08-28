#include "ft_ping.h"

// https://stackoverflow.com/questions/36105998/icmp-header-and-ip-header-checksum-calculations
unsigned short calculate_icmp_checksum(unsigned short *buf, size_t length)
{
	long			sum = 0;

    while (length > 1) { sum += *buf++; length -= 2; }

	if (length > 0) { sum += *(unsigned char *)buf; }

	while (sum >> 16) { sum = (sum & 0xFFFF) + (sum >> 16); }

    return ~sum;
}

void	add_to_list(t_ping_list **list, t_ping_list *new)
{
	t_ping_list	*tmp = *list;
	if (!*list) { *list = new; }
	else
	{
		while (tmp->next) { tmp = tmp->next; }
		tmp->next = new;
	}
}

void	clear_list(t_ping_list *list)
{
	while (list)
	{
		t_ping_list	*tmp = list->next;
		free(list);
		list = tmp;
	}
	list = NULL;
}

double	sqrt(double x)
{
	double result = 1.0;
	for (int i = 0; i < 100; ++i) { result = (result + x / result) / 2; }
	return result;
}

double	pow(double x, double y)
{
	if (x == 0) { return 0.0; }
	if (y == 0) { return 1.0; }
	double result = 1.0;
	for (int i = 0; i < y; ++i) { result *= x; }
	return result;
}
