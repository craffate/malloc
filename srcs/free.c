/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/25 14:03:00 by craffate          #+#    #+#             */
/*   Updated: 2020/06/30 17:45:45 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static void				coalesce_next(t_chunk *chunk)
{
	if ((chunk->size & FREE_MASK) <= SMALL && chunk->next &&
	chunk->next->size & ~FREE_MASK)
	{
		chunk->size = chunk->size + (chunk->next->size & FREE_MASK) +
		sizeof(t_chunk);
		chunk->next = chunk->next->next;
	}
}

static void				coalesce_prev(t_chunk *chunk)
{
	t_chunk				*idx;

	idx = NULL;
	if ((chunk->size & FREE_MASK) <= TINY)
		idx = g_arena->tiny->head;
	else if ((chunk->size & FREE_MASK) <= SMALL)
		idx = g_arena->small->head;
	while (idx && idx->next != chunk)
		idx = idx->next;
	if (idx && idx->size & ~FREE_MASK)
	{
		idx->next = chunk->next;
		idx->size = idx->size + (chunk->size & FREE_MASK) + sizeof(t_chunk);
	}
}

void					free(void *ptr)
{
	t_chunk				*chunk;

	chunk = (t_chunk *)((char *)ptr - sizeof(t_chunk));
	chunk->size += 0x1;
	coalesce_next(chunk);
	coalesce_prev(chunk);
}
