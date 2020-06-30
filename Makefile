ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so
LIBFT_NAME = libft.a

CC = clang

CFLAGS = -Wall -Wextra -Werror
LFLAGS = -shared

ifneq (,$(findstring Linux,$(HOSTTYPE)))
	CFLAGS += -fPIC
endif

INCS_PATH = includes/
OBJS_PATH = objs/
SRCS_PATH = srcs/
LIBFT_PATH = libft/
LIBFT_INCS_PATH = $(addprefix $(LIBFT_PATH), $(INCS_PATH))

SRCS_FILES = malloc free debug realloc calloc

SRCS = $(addsuffix .c, $(addprefix $(SRCS_PATH), $(SRCS_FILES)))
OBJS = $(addsuffix .o, $(addprefix $(OBJS_PATH), $(SRCS_FILES)))
LIBFT = $(addprefix $(LIBFT_PATH), $(LIBFT_NAME))

all: $(LIBFT) $(OBJS_PATH) $(NAME) $(LINK)

$(OBJS_PATH):
	mkdir $(OBJS_PATH)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LFLAGS) -L$(LIBFT_PATH) -lft

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCS_PATH) -I$(LIBFT_INCS_PATH)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(LINK):
	ln -sf $(NAME) $(LINK)

clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(OBJS_PATH)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -f $(NAME)
	rm -f $(LINK)

re: fclean all
