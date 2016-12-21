#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct node{ //Structure qui contient les informations d'un noeud : son nom, son nombre de ports d'entrée et de sortie, et et le nom de ses ports d'entrée et de sortie
	char* nom;
	int nbPortIn;
	int nbPortOut;
	char** portIn;
	char** portOut;
}Node;

typedef struct cfg{ //Structure qui contient les informations issus du fichier de configuration : le nom des noeuds, bin, publish, listen et arguments
	char* noeud;
	char* bin;
	char* publish;
	char* listen;
	char* args;
}Cfg;

typedef struct veriFifo{ //Structure qui sert à vérifier si tous les fifo sont bien ouverts en lecture et en écriture
	char* nom; //nom du fifo
	int mode;  // mode = 0 si le fifo est associé à listen, 1 si à publish
	int verifie; //verifie est egal a 1 si le fifo a une entrée et une sortie, 0 sinon
}VeriFifo;

#endif /* STRUCTURE_H */
