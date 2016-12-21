#include "structure.h"

#ifndef TELE_H
#define TELE_H

#define BUFFSIZE 1024 // Nombre maximal de lignes pour le fichier cfg

#endif 

void creerFifo(char* nom); 
Node* creerNode(char* nom); 
int ajouterPort(Node* noeud, int mode, char* nom); 
int ajouterPortEntree(Node* noeud, char* nom);
int ajouterPortSortie(Node* noeud, char* nom);
void supprimerNode(Node* noeud); 
void copieChaine(char* chaine_src, char* chaine_dest);
int testTele(int argc, char* argv[], char* publish, Node* noeud);
void constructionBuff(char* buff, char* instruction, char* valeur1, char* valeur2);
int test_entree(char* buff, char* instruction);

