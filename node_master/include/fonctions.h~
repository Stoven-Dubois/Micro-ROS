#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include "structure.h"

#ifndef MASTER_H
#define MASTER_H

#define BUFFSIZE 1024 // Nombre maximal de lignes pour le fichier cfg

#endif 

Cfg** creerCfg(char* path, int* nb);
void remplissageNoeud(int* nb, Cfg** config, char* line);
void remplissageBin(int* nb, Cfg** config, char* line);
void remplissagePublish(int* nb, Cfg** config, char* line);
void remplissageListen(int* nb, Cfg** config, char* line);
void remplissageArgs(int* nb, Cfg** config, char* line);
void supprimerCfg(Cfg** cfg, int nb);
char* getIndicateur(char *str);
char** parse(char* str);
int hidenchar(char *str, char c);
char **my_comp_tab(char *mem, char **tab, char *space);
char **my_str_to_wordtab(char *str, char *space);

char* getArgs(char *str);
char* getArg1(char *str);
char* getArg2(char *str);
void prettyPrint(Cfg** config, int* nb);
char* existCheck(char* path);
char* getBinName(char *str);
char* getFifo(char* str, int mode); //Fonction qui extrait le nom du fifo lors de la lecture du fichier de configuration
int majFifo(VeriFifo** verififo, char* nom, int nb, int mode); //Fonction qui met à jour la valeur "verifie" de la structure VeriFifo
int testFifo(VeriFifo** verififo, int nb); //Fonction qui regarde si les fifo ont bien ete verifies
int constructionFifo(VeriFifo** verififo, char* chaineMode,int* nbLecture, int* nbFifo, int mode); //Fonction qui crée les éléments de type VeriFifo en lisant les fichiers de configuration

void creerFifo(char* nom);
void supprimerVeriFifo(VeriFifo** verififo, int nb);
Node* creerNode(char* nom);
int ajouterPort(Node* noeud, int mode, char* nom);
int ajouterPortEntree(Node* noeud, char* nom);
int ajouterPortSortie(Node* noeud, char* nom);
void supprimerNode(Node* noeud); //libère la mémoire
void usageMaster();
void copieChaine(char* chaine_src, char* chaine_dest);//Fonction pour ajouter le chemin menant à la racine du dossier du programme à un nom de fichier



