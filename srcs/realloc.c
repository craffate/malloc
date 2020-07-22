/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 11:35:42 by craffate          #+#    #+#             */
/*   Updated: 2020/07/22 06:01:52 by craffate         ###   ########.fr       */
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

static void				copy_data(char *dst, char *src)
{
	unsigned int		idx;
	size_t				dst_size;
	size_t				src_size;

	idx = -1u;
	dst_size = (size_t)((t_chunk *)(dst - sizeof(t_chunk)))->size;
	src_size = (size_t)((t_chunk *)(src - sizeof(t_chunk)))->size;
	if (dst_size < src_size)
	{
		while (++idx < dst_size)
			dst[idx] = src[idx];
	}
	else
	{
		while (++idx < src_size)
			dst[idx] = src[idx];
	}
}

void					*realloc(void *ptr, size_t size)
{
	void				*p;

	p = NULL;
	free(ptr);
	p = malloc(size);
	if (ptr && check_range(ptr))
		copy_data(p, ptr);
	return (p);
}
