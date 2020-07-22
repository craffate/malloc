/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 17:36:31 by craffate          #+#    #+#             */
/*   Updated: 2020/07/22 17:37:54 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

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
