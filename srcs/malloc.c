/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:34:31 by craffate          #+#    #+#             */
/*   Updated: 2020/06/30 10:06:54 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static t_page			*find_head(size_t size)
{
	t_page				*ret;

	ret = NULL;
	if (size <= TINY)
		ret = g_arena->tiny;
	else if (size <= SMALL)
		ret = g_arena->small;
	else
		ret = g_arena->large;
	return (ret);
}

static void				*shrink_chunk(size_t size, t_page *page)
{
	void				*ret;

	ret = ft_memcpy(page->top, &size, sizeof(size_t));
	ft_memcpy((char *)page->top +
	(char)sizeof(size_t) + size, &size, sizeof(size_t));
	page->top = ((char *)page->top) + ((char)sizeof(size_t) * 2) + size;
	page->top_size = page->top_size - (size + sizeof(size_t) * 2);
	return (ret);
}

static t_page			*find_page(size_t size)
{
	t_page				*ret;

	ret = find_head(size);
	while (ret && ret->top_size < (size + (sizeof(size_t) * 2)))
		ret = ret->next;
	return (ret);
}

static t_page			*map_page(size_t size)
{
	t_page				*ret;

	size += sizeof(size_t) * 2;
	size *= size > (SMALL + sizeof(size_t) * 2) ? 1 : MAX_ALLOC;
	size += sizeof(size_t);
	size += sizeof(t_page);
	size = ft_roundup(size, getpagesize());
	if ((ret = (t_page *)mmap(0, size, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		ret = NULL;
	ret->size = size;
	ret->top_size = size - sizeof(t_page) - sizeof(size_t);
	ret->top = ((char *)ret) + (char)sizeof(t_page) + (char)sizeof(size_t);
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
	if (!(page = find_page(size)))
		page = append_page(find_head(size), map_page(size));
	ret = shrink_chunk(size, page);
	return (ret + sizeof(size_t));
}
