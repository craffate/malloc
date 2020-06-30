/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 19:29:28 by craffate          #+#    #+#             */
/*   Updated: 2020/06/30 19:40:36 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void					*calloc(size_t count, size_t size)
{
	void				*ret;

	ret = malloc(count * size);
	ft_bzero(ret, count * size);
	return (ret);
}
