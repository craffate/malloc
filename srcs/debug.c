/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:58:13 by craffate          #+#    #+#             */
/*   Updated: 2020/07/23 07:23:54 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static void				print_char(unsigned char c)
{
	unsigned char		byte[2];

	byte[1] = ("0123456789abcdef")[c % 16];
	byte[0] = ("0123456789abcdef")[c / 16];
	if ((c > 0x20 && c < 0x7F) && DEBUG_COLOR)
		ft_putstr(RED);
	else if (c && DEBUG_COLOR)
		ft_putstr(YELLOW);
	write(1, byte, 2);
	ft_putstr(EOC);
}

static char				*dump_page_sec(char *s, char *page_idx)
{
	ft_putstr(" |");
	ft_putstr(s);
	ft_putendl("|");
	return (page_idx + 0x10);
}

static void				dump_page(t_page *page)
{
	char				*page_idx;
	char				s[17];
	unsigned int		idx;

	page_idx = (char *)page;
	s[16] = 0;
	while (page_idx < ((char *)page + (page->size - page->top_size)))
	{
		idx = -1u;
		print_addr(page_idx);
		ft_putstr("  ");
		while (++idx < 0x10)
		{
			print_char(((unsigned char *)page_idx)[idx]);
			if (idx == 0x7)
				ft_putchar(' ');
			ft_putchar(' ');
			if (page_idx[idx] && (page_idx[idx] > 0x20 && page_idx[idx] < 0x7F))
				s[idx] = page_idx[idx] ? page_idx[idx] : '.';
			else
				s[idx] = '.';
		}
		page_idx = dump_page_sec(s, page_idx);
	}
	ft_putstr("\n\n");
}

void					show_alloc_mem_ex(void)
{
	t_page				*page_idx;

	if (g_arena)
	{
		page_idx = g_arena->tiny;
		while (page_idx)
		{
			dump_page(page_idx);
			page_idx = page_idx->next;
		}
		page_idx = g_arena->small;
		while (page_idx)
		{
			dump_page(page_idx);
			page_idx = page_idx->next;
		}
		page_idx = g_arena->large;
		while (page_idx)
		{
			dump_page(page_idx);
			page_idx = page_idx->next;
		}
	}
}

void					show_alloc_mem(void)
{
	t_page				*page_idx;

	if (g_arena)
	{
		page_idx = g_arena->tiny;
		while (page_idx)
		{
			print_page(page_idx);
			page_idx = page_idx->next;
		}
		page_idx = g_arena->small;
		while (page_idx)
		{
			print_page(page_idx);
			page_idx = page_idx->next;
		}
		page_idx = g_arena->large;
		while (page_idx)
		{
			print_page(page_idx);
			page_idx = page_idx->next;
		}
	}
}
