#include "structure.h"

#ifndef FONCTIONS_H
#define FONCTIONS_H

void creerFifo(char* nom); 
Node* creerNode(char* nom);
int ajouterPort(Node* noeud, int mode, char* nom); 
int ajouterPortEntree(Node* noeud, char* nom);
int ajouterPortSortie(Node* noeud, char* nom);
void supprimerNode(Node* noeud);
void supprimerLogger(Logger* arg);
void usageLogger();
void copieChaine(char* chaine_src, char* chaine_dest);
int testLogger(int argc,char** argv, char* listen, char* log, char* publish, Node* noeud); 

#endif




