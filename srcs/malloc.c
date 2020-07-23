/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:34:31 by craffate          #+#    #+#             */
/*   Updated: 2020/07/23 07:50:53 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static void				*shrink_chunk(size_t size, t_page *page)
{
	void				*ret;
	t_chunk				chunk;
	t_chunk				*link;

	chunk.size = size;
	chunk.next = NULL;
	link = page->head;
	ret = ((char *)page + (page->size - page->top_size));
	while (link->next)
		link = link->next;
	link->next = ret;
	ft_memcpy(ret, &chunk, sizeof(t_chunk));
	page->top_size = page->top_size - (size + sizeof(t_chunk));
	return (ret);
}

static t_page			*map_page(size_t size)
{
	t_page				*ret;

	size += sizeof(t_chunk);
	size *= size > (SMALL + sizeof(t_chunk)) ? 1 : MAX_ALLOC;
	size += sizeof(t_page);
	size = ft_roundup(size, getpagesize());
	if ((ret = (t_page *)mmap(0, size, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		ret = NULL;
	ret->size = size;
	ret->top_size = size - sizeof(t_page);
	ret->head = (void *)((char *)ret) + sizeof(t_page);
	ret->next = NULL;
	return (ret);
}

static t_arena			*map_arena(void)
{
	t_arena				*ret;

	if ((ret = (t_arena *)mmap(0, sizeof(t_arena), PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		ret = NULL;
	ret->tiny = map_page(TINY);
	ret->small = map_page(SMALL);
	ret->large = NULL;
	return (ret);
}

static t_page			*append_page(t_page *p_head, t_page *page)
{
	t_page				*ret;

	ret = p_head;
	if (!ret)
	{
		g_arena->large = page;
		ret = g_arena->large;
	}
	else
	{
		while (ret->next)
			ret = ret->next;
		ret->next = page;
		ret = ret->next;
	}
	return (ret);
}

void					*malloc(size_t size)
{
	void				*ret;
	t_page				*page;

	if (!g_arena)
		g_arena = map_arena();
	size = ft_roundup(size, 16);
	if (!size)
		return (NULL);
	if (!(ret = find_free(size)))
	{
		if (!(page = find_page(size)))
			page = append_page(find_head(size), map_page(size));
		ret = shrink_chunk(size, page);
	}
	return (ret + sizeof(t_chunk));
}
