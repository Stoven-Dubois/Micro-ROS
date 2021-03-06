#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include "fastsim_c.h"
#include <X11/Xlib.h>
#include <pthread.h>

#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct node{//Structure qui contient les informations d'un noeud : son nom, son nombre de ports d'entrée et de sortie, et et le nom de ses ports d'entrée et de sortie
	char* nom;
	int nbPortIn;
	int nbPortOut;
	char** portIn;
	char** portOut;
	}Node;
	
typedef struct simu{ //structure qui contient les informations pour les threads de node_simu
	char* buff;
	int fd;	//fifo qu'on écoute
	int bumper_r;
	int bumper_l;
	int fp1;  //fifo pour le premier bumper
	int fp2; //fifo pour le deuxième bumper
	robot_t* robot;
	map_t* carte;
	display_t* affichage;
	float* vr; //vitesses des moteurs du robot
	float* vl;
	int test;  //test pour savoir si l'utilisateur veut arrêter le programme
	int test_lecture; //test pour savoir si l'utilisateur a entré une nouvelle commande
}Simu;


#endif /* STRUCTURE_H */


