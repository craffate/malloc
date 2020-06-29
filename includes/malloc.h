/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:05:38 by craffate          #+#    #+#             */
/*   Updated: 2020/06/29 11:48:06 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include "libft.h"

# define MAX_ALLOC 100

# define TINY 16
# define SMALL 128

# define DEBUG_COLOR 1

# define FREE_MASK 0xFFFFFFFFFFFFFFF0

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define EOC "\x1b[0m"

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
void					show_alloc_mem_ex(void);

#endif
