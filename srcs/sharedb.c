/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sharedb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 07:47:21 by craffate          #+#    #+#             */
/*   Updated: 2020/07/23 07:50:27 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

int						find_range(t_page *ar_head, uintptr_t ptr)
{
	int					ret;
	t_page				*ar_idx;

	ret = 0;
	ar_idx = ar_head;
	while (ar_idx && !ret)
	{
		if ((uintptr_t)ar_idx < ptr &&
		(uintptr_t)((char *)ar_idx + ar_idx->size) > ptr)
			ret = 1;
		ar_idx = ar_idx->next;
	}
	if (ret)
	{
		if (ar_head == g_arena->small)
			ret = 2;
		else if (ar_head == g_arena->large)
			ret = 3;
	}
	return (ret);
}

t_page					*find_head(size_t size)
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

t_page					*find_page(size_t size)
{
	t_page				*ret;

	ret = find_head(size);
	if ((ret = find_head(size)) == g_arena->large)
		return (NULL);
	while (ret && ret->top_size < (size + sizeof(t_chunk)))
		ret = ret->next;
	return (ret);
}

void					*find_free(size_t size)
{
	void				*ret;
	t_page				*page_idx;
	t_chunk				*chunk_idx;

	ret = NULL;
	page_idx = find_head(size);
	while (page_idx && !ret)
	{
		chunk_idx = page_idx->head;
		while (chunk_idx)
		{
			if ((chunk_idx->size & FREE_MASK) >= size &&
			(chunk_idx->size & ~FREE_MASK))
			{
				ret = (void *)chunk_idx;
				((t_chunk *)ret)->size -= 0x1;
			}
			chunk_idx = chunk_idx->next;
		}
		page_idx = page_idx->next;
	}
	return (ret);
}
