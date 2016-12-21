##
## Makefile for Makefile in /home/sasl/eleves/rob/3410057/Documents/infosys/projet/programme/
## 
## Made by Alexis Stoven-Dubois
## 
##

CC	=	gcc

NAME	=	node_echo

CFLAGS	=	-W -Wall -lpthread -I./include/ -g3

SRC	=	src/main.c	\
	     	src/structure.c 	\
	     	src/fonctions.c

RM	=	rm -f

OBJS	=	$(SRC:.c=.o)

all	:	$(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME)	:	$(OBJS)
		$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		$(RM) $(NAME)

re	:	fclean clean all
