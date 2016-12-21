##
## Makefile for zappy in /home/calatr_m/cpp_rendu/PSU/PSU_2014_zappy
## 
## Made by Alexis Stoven-Dubois
## 
## compile tout

all	:	node_logger node_echo node_master node_simu node_tele

node_logger	:
		make -C ./node_logger
		
node_echo	:
		make -C ./node_echo
		
node_master	:
		make -C ./node_master

node_simu	:
		make -C ./node_simu

node_tele	:
		make -C ./node_tele

clean	:
		make -C ./node_logger clean
		make -C ./node_echo clean
		make -C ./node_master clean
		make -C ./node_simu clean
		make -C ./node_tele clean

fclean	:
		make -C ./node_logger fclean
		make -C ./node_echo fclean
		make -C ./node_master fclean
		make -C ./node_simu fclean
		make -C ./node_tele fclean

re	:	fclean all

.PHONY	:	all node_echo node_logger node_master node_simu node_tele clean fclean re
