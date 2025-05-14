CC = cc
# CFLAGS = -Wall -Wextra -Werror
NAME = libft.a

SRCM = ft_isalnum.c ft_isdigit.c ft_memchr.c  ft_memmove.c ft_putendl_fd.c ft_split.c   ft_striteri.c   ft_strlcpy.c ft_strncmp.c ft_strtrim.c ft_toupper.c \
	   ft_bzero.c ft_isalpha.c ft_isprint.c ft_memcmp.c  ft_memset.c  ft_putnbr_fd.c  ft_strchr.c  ft_strjoin.c ft_strlen.c  ft_strnstr.c ft_substr.c ft_strncat.c \
	   ft_calloc.c ft_isascii.c ft_itoa.c    ft_memcpy.c  ft_putchar_fd.c ft_putstr_fd.c  ft_strdup.c  ft_strlcat.c ft_strmapi.c ft_strrchr.c ft_tolower.c ft_atoi.c ft_strcat.c ft_strjoin2.c ft_split1.c

SRCB = ft_lstnew_bonus.c ft_lstlast_bonus.c ft_lstadd_front_bonus.c  ft_lstadd_back_bonus.c ft_lstsize_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c 
OBJS = $(SRCM:.c=.o)
OBJB = $(SRCB:.c=.o)

all: $(NAME) $(OBJB)

$(NAME) : $(OBJS) 

%.o: %.c libft.h build_progress
	@$(CC) $(CFLAGS) -c $<
	@ar -rcs $(NAME) $@


build_progress:
	@echo "minishell is building ."
	@echo "minishell is building .."
	@echo "minishell is building ..."

bonus : $(OBJB)

clean:
	@rm -f $(OBJS) $(OBJB)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY : clean