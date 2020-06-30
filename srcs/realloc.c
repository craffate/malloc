/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 11:35:42 by craffate          #+#    #+#             */
/*   Updated: 2020/06/30 11:52:54 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void					*realloc(void *ptr, size_t size)
{
	void				*p;
	size_t				ptr_size;

	ptr_size = (size_t)*(((char *)ptr) - ((char)sizeof(size_t)));
	free(ptr);
	p = malloc(size);
	p = ft_memcpy(p, ptr, ptr_size);
	return (p);
}
