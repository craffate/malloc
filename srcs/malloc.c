/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:34:31 by craffate          #+#    #+#             */
/*   Updated: 2020/06/25 06:22:01 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

t_arena					*g_arena;

static void				*shrink_chunk(size_t size, t_page *page)
{
	void				*ret;

	ret = ft_memcpy(page->top, &size, sizeof(size_t));
	page->top = ((char *)page->top) + ((char)sizeof(size_t)) + ((char)size);
	page->top_size = page->top_size - (size + sizeof(size_t));
	return (ret);
}

static t_page			*find_page(size_t size)
{
	t_page				*ret;

	if (size <= TINY)
		ret = g_arena->tiny;
	else if (size <= SMALL)
		ret = g_arena->small;
	else
		ret = g_arena->large;
	while (ret && ret->top_size < (size + sizeof(size_t)))
		ret = ret->next;
	return (ret);
}

static t_page			*map_page(size_t size)
{
	t_page				*ret;

	size *= MAX_ALLOC;
	size *= getpagesize();
	size += sizeof(t_page);
	if ((ret = (t_page *)mmap(0, size, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		ret = NULL;
	ret->size = size;
	ret->top_size = size - sizeof(t_page);
	ret->top = ((char *)ret) + sizeof(t_page);
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

void					*malloc(size_t size)
{
	void				*ret;
	t_page				*page;

	if (!g_arena)
		g_arena = map_arena();
	size = ft_roundup(size, 16);
	page = find_page(size);
	ret = shrink_chunk(size, page);
	return (ret + sizeof(size_t));
}
