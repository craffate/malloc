/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 17:36:31 by craffate          #+#    #+#             */
/*   Updated: 2020/07/22 18:06:40 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void					print_addr(void *ptr)
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

void					print_range(void *p, void *p2)
{
	print_addr(p);
	ft_putstr(" - ");
	print_addr(p2);
}

void					print_chunk(t_chunk *chunk)
{
	if (chunk->size & ~FREE_MASK && DEBUG_COLOR)
		ft_putstr(GREEN);
	print_range(chunk,
	((char *)chunk + (chunk->size & FREE_MASK) + sizeof(t_chunk)));
	ft_putstr(" : ");
	ft_putnbr(chunk->size & FREE_MASK);
	ft_putstr(" bytes");
	ft_putchar('\n');
	ft_putstr(EOC);
}

void					print_page(t_page *page)
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
	ft_putstr("\n\n");
}

int						check_range(void *ptr)
{
	int					ret;
	uintptr_t			ptr_idx;
	t_page				*idx;

	ret = 0;
	ptr_idx = (uintptr_t)ptr;
	idx = g_arena->tiny;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx &&
		(uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 1;
		idx = idx->next;
	}
	idx = g_arena->small;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx &&
		(uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 2;
		idx = idx->next;
	}
	idx = g_arena->large;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx &&
		(uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 3;
		idx = idx->next;
	}
	return (ret);
}
