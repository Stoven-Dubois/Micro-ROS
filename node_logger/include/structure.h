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


typedef struct node{ //Structure qui contient les informations d'un noeud : son nom, son nombre de ports d'entrée et de sortie, et et le nom de ses ports d'entrée et de sortie
	char* nom;
	int nbPortIn;
	int nbPortOut;
	char** portIn;
	char** portOut;
	}Node;

typedef struct logger{ //Structure qui contient les informations pour les threads du logger : un buffer pour stocker les données lues dans le fifo, un descripteur du fichier fifo ouvert, le fichier dans lequel écrire les données, une variable test pour vérifier si l'utilisateur veut terminer le programme, un descripteur du fichier fifo dans lequel on publie si voulu par l'utilisateur
	char* buff;
	int fd;	
	FILE* log;
	int test;
	int fdPublish;
}Logger;

#endif /* STRUCTURE_H */


