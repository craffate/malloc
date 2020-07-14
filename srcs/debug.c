/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:58:13 by craffate          #+#    #+#             */
/*   Updated: 2020/07/14 07:14:18 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static void				print_addr(void *ptr)
{
	size_t				addr_size;
	char				addr[17];
	ptrdiff_t			ptrdiff;

	addr_size = 0;
	ptrdiff = ptr - NULL;
	while (ptrdiff /= 10)
		++addr_size;
	addr[addr_size] = 0;
	ptrdiff = ptr - NULL;
	while (addr_size--)
	{
		addr[addr_size] = ("0123456789ABCDEF")[ptrdiff % 16];
		ptrdiff /= 16;
	}
	if (addr[0])
	{
		ft_putstr("0x");
		ft_putstr(addr);
	}
	else
		ft_putstr("NULL");
}

static void				print_range(void *p, void *p2)
{
	print_addr(p);
	ft_putstr(" - ");
	print_addr(p2);
}

static void				print_chunk(t_chunk *chunk)
{
	if (chunk->size & ~FREE_MASK && DEBUG_COLOR)
		ft_putstr(GREEN);
	print_range(chunk, ((char *)chunk + (chunk->size & FREE_MASK) + sizeof(t_chunk)));
	ft_putstr(" : ");
	ft_putnbr(chunk->size & FREE_MASK);
	ft_putstr(" bytes");
	ft_putchar('\n');
	ft_putstr(EOC);
}

static void				print_page(t_page *page)
{
	t_chunk				*chunk;

	if (page)
	{
		chunk = page->head;
		ft_putstr("Page size: ");
		ft_putnbr(page->size);
		ft_putchar('\n');
		ft_putstr("Top size: ");
		ft_putnbr(page->top_size);
		ft_putchar('\n');
		ft_putstr("Page range: ");
		print_range(page, ((char *)page + page->size));
		ft_putchar('\n');
		ft_putstr("Page next: ");
		print_addr(page->next);
		ft_putchar('\n');
		while (chunk)
		{
			print_chunk(chunk);
			chunk = chunk->next;
		}
	}
}

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
		ft_putstr(" |");
		ft_putstr(s);
		ft_putchar('|');
		ft_putchar('\n');
		page_idx += 0x10;
	}
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
			ft_putstr("\n\n");
			page_idx = page_idx->next;
		}
		page_idx = g_arena->small;
		while (page_idx)
		{
			dump_page(page_idx);
			ft_putstr("\n\n");
			page_idx = page_idx->next;
		}
		page_idx = g_arena->large;
		while (page_idx)
		{
			dump_page(page_idx);
			ft_putstr("\n\n");
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
			ft_putstr("\n\n");
			page_idx = page_idx->next;
		}
		page_idx = g_arena->small;
		while (page_idx)
		{
			print_page(page_idx);
			ft_putstr("\n\n");
			page_idx = page_idx->next;
		}
		page_idx = g_arena->large;
		while (page_idx)
		{
			print_page(page_idx);
			ft_putstr("\n\n");
			page_idx = page_idx->next;
		}
	}
}
