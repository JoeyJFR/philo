# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 13:14:28 by zjiang            #+#    #+#              #
#    Updated: 2025/03/15 08:58:56 by zjiang           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc

SRC=main.c parsing.c init.c dinner.c getter_setter.c sync.c monitor.c\
utils1.c utils2.c\
guarding.c

OBJDIR=.obj

OBJ=$(SRC:%.c=$(OBJDIR)/%.o)

CFLAGS= -Wextra -Wall -Werror -g3

NAME=philo

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -pthread

$(OBJDIR)/%.o : %.c Makefile philo.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJDIR) :
	mkdir -p $(OBJDIR)

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
