/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:58:13 by craffate          #+#    #+#             */
/*   Updated: 2020/06/25 05:48:45 by craffate         ###   ########.fr       */
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

static void				print_chunk(void *chunk)
{
	print_addr(chunk);
	ft_putstr(" - ");
	print_addr(((char *)chunk) + *((size_t *)chunk) + sizeof(size_t));
	ft_putstr(" : ");
	ft_putnbr(*((size_t *)chunk));
	ft_putstr(" bytes");
	ft_putchar('\n');
}

static void				print_page(t_page *page)
{
	void				*chunk;

	if (page)
	{
		chunk = ((char *)page) + sizeof(t_page);
		ft_putstr("Page size: ");
		ft_putnbr(page->size);
		ft_putchar('\n');
		ft_putstr("Top size: ");
		ft_putnbr(page->top_size);
		ft_putchar('\n');
		ft_putstr("Top range: ");
		print_addr(page->top);
		ft_putstr(" - ");
		print_addr(((char *)page->top) + page->top_size);
		ft_putchar('\n');
		ft_putstr("Page range: ");
		print_addr(page);
		ft_putstr(" - ");
		print_addr(((char *)page) + page->size);
		ft_putchar('\n');
		ft_putstr("Page next: ");
		print_addr(page->next);
		ft_putchar('\n');
		while (chunk != page->top)
		{
			print_chunk(chunk);
			chunk = ((char *)chunk) + (*(char *)chunk) + sizeof(size_t);
		}
	}
}

static void				print_char(unsigned char c)
{
	unsigned char		byte[2];
	unsigned char		base[16] = "0123456789abcdef";

	byte[1] = base[c % 16];
	byte[0] = base[c / 16];
	write(1, byte, 2);
}

static void				dump_page(t_page *page)
{
	char				*page_idx;
	char				s[17];
	unsigned int		idx;

	page_idx = (char *)page;
	s[16] = 0;
	while (page_idx < ((char *)page->top))
	{
		idx = -1u;
		print_addr(page_idx);
		ft_putstr(": ");
		while (++idx < 0x10)
		{
			print_char(((unsigned char *)page_idx)[idx]);
			ft_putchar(' ');
			if (page_idx[idx] && (page_idx[idx] > 0x20 && page_idx[idx] < 0x7F))
				s[idx] = page_idx[idx] ? page_idx[idx] : '.';
			else
				s[idx] = '.';
		}
		ft_putstr(" | ");
		ft_putstr(s);
		ft_putchar('\n');
		page_idx += 0x10;
	}
}

void					show_alloc_mem_ex(void)
{
	t_page				*page_idx;

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

void					show_alloc_mem(void)
{
	t_page				*page_idx;

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
