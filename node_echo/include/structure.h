#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct node{  //Structure qui contient les informations d'un noeud : son nom, son nombre de ports d'entrée et de sortie, et et le nom de ses ports d'entrée et de sortie
	char* nom;
	int nbPortIn;
	int nbPortOut;
	char** portIn;
	char** portOut;
}Node;
	
typedef struct echo{  //Structure qui contient les informations pour les threads du echo : un buffer pour stocker les données à écrire dans le fifo, un message à écrire un descripteur du fichier fifo ouvert, l'intervalle de temps régissant la publication du message, une variable test pour vérifier si l'utilisateur veut terminer le programme, 
	char* buff;
	char* msg;
	int fd;
	float intervalle;
	int test;
}Echo;

#endif /* STRUCTURE_H */


