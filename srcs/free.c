/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/25 14:03:00 by craffate          #+#    #+#             */
/*   Updated: 2020/06/27 20:50:42 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void					free(void *ptr)
{
	size_t				*p;

	p = (size_t *)(((char *)ptr) - ((char)sizeof(size_t)));
	*p += 0x1;
}
