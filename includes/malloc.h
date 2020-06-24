/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:05:38 by craffate          #+#    #+#             */
/*   Updated: 2020/06/24 05:39:44 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

typedef struct			s_arena
{
	struct s_page		*tiny;
	struct s_page		*small;
	struct s_page		*large;
}						t_arena;

typedef struct			s_page
{
	size_t				size;
	size_t				top_size;
	void				*top;
	struct s_page		*next;
}						t_page;

extern struct s_arena	*g_arena;

void					*malloc(size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);

#endif
