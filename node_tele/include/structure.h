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

typedef struct node{
	char* nom;
	int nbPortIn;
	int nbPortOut;
	char** portIn;
	char** portOut;
	}Node;
	
typedef struct fifo{
	char* nom;
	int fd;
	}Fifo;

typedef struct arg{
	char* buff;
	int fd;	
	FILE* log;
	int test;
}Arg;

typedef struct cfg{
	char* noeud;
	char* bin;
	char* publish;
	char* listen;
	char* args;
}Cfg;
/*
typedef struct simu{
	char* buff;
	int fd;	
	int bumper_r;
	int bumper_l;
	FILE* fp1;
	FILE* fp2;
	robot_t* robot;
	map_t* carte;
	display_t* affichage;
	float* vr;
	float* vl;
}Simu;
*/

#endif /* STRUCTURE_H */


