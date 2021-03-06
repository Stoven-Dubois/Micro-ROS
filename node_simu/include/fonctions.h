#include "structure.h"

#ifndef SIMU_H
#define SIMU_H

#define BUFFSIZE 1024 // Nombre maximal de lignes pour le fichier cfg

void creerFifo(char* nom); 
Node* creerNode(char* nom);
int ajouterPort(Node* noeud, int mode, char* nom); 
int ajouterPortEntree(Node* noeud, char* nom);
int ajouterPortSortie(Node* noeud, char* nom);
void supprimerNode(Node* noeud);
void usageSimu();
void copieChaine(char* chaine_src, char* chaine_dest);
int testSimu(int argc, char** argv, char* listen, char* publish1, char* publish2, char* carte, float* x, float* y, Node* noeud);
int lectureInstruction(char* buff, char* instruction, char* valeur1, char* valeur2);
void remplissageInstruction(char* buff, char* instruction, int*i, int* testInstruction);
void remplissageValeur1(char* buff, char* valeur1, int*i, int* testInstruction);
void remplissageValeur2(char* buff, char* valeur2, int*i, int* testInstruction);
void lectureSimu(Simu* arg1);
int ordreSimu(char* instruction, float vitesse1, float vitesse2,float* vr, float* vl);
void inversion(char* instruction, float* valeur1, float* valeur2);

#endif // SIMU_H
