/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 11:35:42 by craffate          #+#    #+#             */
/*   Updated: 2020/07/01 10:55:09 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static int				check_range(void *ptr)
{
	int					ret;
	uintptr_t			ptr_idx;
	t_page				*idx;

	ret = 0;
	ptr_idx = (uintptr_t)ptr;
	idx = g_arena->tiny;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx && (uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 1;
		idx = idx->next;
	}
	idx = g_arena->small;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx && (uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 1;
		idx = idx->next;
	}
	idx = g_arena->large;
	while (idx && !ret)
	{
		if ((uintptr_t)idx < ptr_idx && (uintptr_t)((char *)idx + idx->size) > ptr_idx)
			ret = 1;
		idx = idx->next;
	}
	return (ret);
}

void					*realloc(void *ptr, size_t size)
{
	void				*p;

	p = NULL;
	free(ptr);
	p = malloc(size);
	if (ptr && check_range(ptr))
		ft_memcpy(p, ptr, size);
	return (p);
}
