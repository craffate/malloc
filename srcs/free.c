/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/25 14:03:00 by craffate          #+#    #+#             */
/*   Updated: 2020/06/30 10:17:42 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_arena					*g_arena;

static int				ptr_issmall(void *ptr)
{
	int					ret;
	t_page				*small;

	ret = 0;
	small = g_arena->small;
	while (!ret && small)
	{
		if (ptr > (void *)((char *)small + sizeof(size_t) + sizeof(t_page)) &&
		ptr < (void *)(((char *)small) + small->size))
			ret = 1;
		small = small->next;
	}
	return (ret);
}

static void				try_coalesce(size_t *ptr_head, size_t *ptr_end)
{
	size_t				*ptr_prev_head;
	size_t				*ptr_prev_end;

	ptr_prev_end = (size_t *)((char *)ptr_head - (char)sizeof(size_t));
	if (*ptr_prev_end & ~FREE_MASK)
	{
		ptr_prev_head = (size_t *)((char *)ptr_prev_end -
		(char)sizeof(size_t) - (*ptr_prev_end & FREE_MASK));
		*ptr_end = ((*ptr_prev_end + *ptr_head) & FREE_MASK) +
		(sizeof(size_t) * 2);
		*ptr_prev_head = ((*ptr_prev_end + *ptr_head) & FREE_MASK) +
		(sizeof(size_t) * 2);
		*ptr_end += 0x1;
		*ptr_prev_head += 0x1;
	}
}

void					free(void *ptr)
{
	size_t				*ptr_head;
	size_t				*ptr_end;

	ptr_head = (size_t *)(((char *)ptr) - ((char)sizeof(size_t)));
	*ptr_head += 0x1;
	ptr_end = (size_t *)(((char *)ptr_head) +
	((char)sizeof(size_t)) + (*ptr_head & FREE_MASK));
	*ptr_end += 0x1;
	if (ptr_issmall(ptr))
		try_coalesce(ptr_head, ptr_end);
}
