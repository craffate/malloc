/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craffate <craffate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 05:05:38 by craffate          #+#    #+#             */
/*   Updated: 2020/07/23 07:50:33 by craffate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include "libft.h"

# define MAX_ALLOC 100

# define TINY 32
# define SMALL 1024

# define DEBUG_COLOR 1

# define FREE_MASK 0xFFFFFFFFFFFFFFF0

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define EOC "\x1b[0m"

typedef struct			s_arena
{
	struct s_page		*tiny;
	struct s_page		*small;
	struct s_page		*large;
}						t_arena;

typedef struct			s_chunk
{
	size_t				size;
	struct s_chunk		*next;
}						t_chunk;

typedef struct			s_page
{
	size_t				size;
	size_t				top_size;
	struct s_chunk		*head;
	struct s_page		*next;
}						t_page;

extern struct s_arena	*g_arena;

void					*malloc(size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);
void					show_alloc_mem_ex(void);
void					*calloc(size_t count, size_t size);
int						check_range(void *ptr);
int						find_range(t_page *ar_head, uintptr_t ptr);
void					print_addr(void *ptr);
void					print_range(void *p, void *p2);
void					print_chunk(t_chunk *chunk);
void					print_page(t_page *page);
void					*find_free(size_t size);
t_page					*find_page(size_t size);
t_page					*find_head(size_t size);

#endif
