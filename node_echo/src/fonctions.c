#include "fonctions.h"


void creerFifo(char* nom){  //créé un fifo nommé "nom", avec les permissions de type "0666"
	if(nom == NULL){
		perror("Erreur de nom");
	}
	if(mkfifo(nom, 0666)== -1){
		perror("Erreur de création du tube");
	}
	return;
}


Node* creerNode(char* nom){ //crée un noeud de nommé "nom", avec 0 ports
	Node* noeud;
	if ((noeud = malloc(sizeof(Node))) == NULL){
		perror("Erreur d'allocation node");
		return NULL;
	}
	if((noeud->nom = malloc(strlen(nom)*sizeof(char))) == NULL){
		perror("Erreur d'allocation nom");
		return NULL;
	}
	strcpy(noeud->nom,nom);
	
	noeud->portIn = malloc(0);
	noeud->portOut = malloc(0);
	
	noeud->nbPortIn = 0;
	noeud->nbPortOut = 0;	
	return noeud;
}

int ajouterPort(Node* noeud, int mode, char* nom){  //ajoute un port de nom nom à la structure noeud, le mode à spécifier est 0 pour les entrées et 1 pour les sorties.
	int retour = 0;
	switch(mode){
		case '0':
			retour = ajouterPortEntree(noeud, nom);
        		break;
    		case '1':
    			retour = ajouterPortSortie(noeud, nom);
        		break;
    		default :
        		break;
	}
	return retour;
}

int ajouterPortEntree(Node* noeud, char* nom){ //fonction qui ajoute un port "nom" d'entrée au noeud "noeud"
	if(noeud->portIn == NULL){
		if((noeud->portIn = malloc(sizeof(char*))) == NULL){
			perror("Erreur d'allocation portIn");
			return -1;
		}
	}
	else{
		if((noeud->portIn = realloc(noeud->portIn, (noeud->nbPortIn+1) * sizeof(char*))) == NULL){
			perror("Erreur d'allocation portIn");
			return -1;
		}
	}
        noeud->portIn[noeud->nbPortIn]=malloc(strlen(nom)*sizeof(char));
        noeud->portIn[noeud->nbPortIn]=nom;
        noeud->nbPortIn++;
	return 0;
}

int ajouterPortSortie(Node* noeud, char* nom){  //fonction qui ajoute un port "nom" de sortie au noeud "noeud"
	if(noeud->portOut == NULL){
		if((noeud->portOut = malloc(sizeof(char*))) == NULL){
			perror("Erreur d'allocation portOut");
			return -1;
		}
	}
	else{
		if((noeud->portOut = realloc(noeud->portOut, (noeud->nbPortOut+1) * sizeof(char*))) == NULL){
			perror("Erreur d'allocation portOut");
			return -1;
		}
	}
        noeud->portOut[noeud->nbPortOut]=malloc(strlen(nom)*sizeof(char));
        noeud->portOut[noeud->nbPortOut]=nom;
        noeud->nbPortOut++;
	return 0;
}


void supprimerNode(Node* noeud){ //libère la mémoire allouée pour un noeud
	int i=0;
	while(noeud->portIn[i] != NULL){
		free(noeud->portIn[i]);
		i++;
	}
	free(noeud->portIn);
	
	i=0;
	while(noeud->portOut[i] != NULL){
		free(noeud->portOut[i]);
		i++;
	}
	free(noeud->portOut);
	free(noeud->nom);
	free(noeud);
}

void supprimerArg(Echo* arg){ //Libère la mémoire allouée pour une variable de type "Echo*"
	free(arg->buff);
	free(arg->msg);
	free(arg);
}

void usageEcho(){  //fonction qui affiche les instructions pour le lancement du node_echo
	printf("./node_echo --publish outport myfifo.fifo --msg Hello --interval nombre_de_secondes \n");
	return;
}

void copieChaine(char* chaine_src, char* chaine_dest){ //fonction qui à partir d'un nom de fichier "chaine_src" à la racine du projet, stoce dans "chaine_dest", le chemin menant à ce fichier depuis le lieu où est l'exécutable
	strcat(chaine_dest, "../");
	strcat(chaine_dest, chaine_src);
}

int testEcho(int argc, char** argv, char* publish, char* msg, float* intervalle, Node* noeud){  //Fonction qui va stocker dans les chaînes de caractères correspondantes les différents arguments que l'utilisateur rentre comme argument du main, peu importe leur ordre : publish (nom du fifo vers lequel on publie), msg (message à envoyer), intervalle (intervalle de temps régissant l'envoi du message, noeud(nom du noeud)
	int i=1;
	while(i<argc){
		if(strcmp(argv[i],"--publish")==0){
			publish = realloc(publish, (strlen(argv[i+2])+3)*sizeof(char));
			copieChaine(argv[i+2], publish);
			if(ajouterPort(noeud, 0, argv[i+1])==-1){return -1;}
			i = i+3;
		}else{
			if(strcmp(argv[i],"--msg")==0){	
				msg = realloc(msg,(strlen(argv[i+1]))*sizeof(char));
				strcpy(msg, argv[i+1]);
				i = i+2;
			}else{
				if(strcmp(argv[i],"--interval")==0){
				*intervalle = atof(argv[i+1]);
				i = i+2;
				}else{
					usageEcho();
					i = argc;
				}
			}
		}
	}
	return 0;
}































