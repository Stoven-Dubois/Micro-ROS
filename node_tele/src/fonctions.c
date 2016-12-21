#include "fonctions.h"

void creerFifo(char* nom){
	if(nom == NULL){
		perror("Erreur de nom");
	}
	if (mkfifo(nom, 0666)== -1){
		perror("Erreur de création du tube");
	}
	return ;
}

Node* creerNode(char* nom){  //Crée un noeud de nommé "nom", avec 0 ports
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

void copieChaine(char* chaine_src, char* chaine_dest){  //Fonction qui à partir d'un nom de fichier "chaine_src" à la racine du projet, stoce dans "chaine_dest", le chemin menant à ce fichier depuis le lieu où est l'exécutable
	strcat(chaine_dest, "../");
	strcat(chaine_dest, chaine_src);
}

void usageTele(int argc){ //fonction qui affiche les instructions pour le lancement du node_tele
	if(argc!=4){
		printf("./node_tele --publish outport myfifo.fifo");
	}else{
		printf("Entrez les commandes pour le robot sous la forme : <Instruction>\n <valeur1> <valeur2 (à rajouter pour l'instruction 'set_v')>\n");
		printf("Instructions : \n   - forward : avancer\n   - backward : reculer\n   - cw : rotation directe\n   - ccw : rotation indirecte\n   - stop : stopper\n   - set_v : fixe manuellement les vitesses des deux moteurs\n");
	}
}

int testTele(int argc, char* argv[], char* publish, Node* noeud){ //Lecture des arguments entrés par l'utilisateur, publish (fifo vers lequel on publie, noeud (nom du noeud)
	if(argc!=4){
		usageTele(argc);
		return -1;
	}
	if(strcmp(argv[1],"--publish")==0){
		publish = realloc(publish, (strlen(argv[2])+3)*sizeof(char));
		copieChaine(argv[3], publish);
		if(ajouterPort(noeud, 0, argv[2])){return -1;}
	}else{
		return -1;
	}
	return 0;
}

void constructionBuff(char* buff, char* instruction, char* valeur1, char* valeur2){ //Construction de la chaîne de caractères buff envoyée dans le fifo à partir des instructions lues
	unsigned int i;
	int cpt;
	for(i=0;i<strlen(instruction);i++){
		buff[i] = instruction[i];
	}
	buff[strlen(instruction)] = ' ';
	cpt = strlen(instruction)+2;
	for(i=0;i<strlen(valeur1);i++){
		buff[cpt+i] = valeur1[i];
	}
	buff[cpt+strlen(valeur1)] = ' ';
	cpt = cpt + strlen(valeur2)+2;
	for(i=0;i<strlen(valeur2);i++){
		buff[cpt+i] = valeur2[i];
	}
	buff[cpt+strlen(valeur2)] = ' ';
}
	
int test_entree(char* buff, char* instruction){ //fonction qui regarde les commandes entrées par l'utilisateur et lui signifie que sa commande est erronée si cela est le cas
	int nombre_valeur = 0, i = 0, signe = 0;
	if(strcmp(instruction,"stop")==0){
		return 0;
	}
	while(buff[i]!='\0'){
		if(buff[i] == '<'){
			signe++;
			nombre_valeur++;
		}
		if(buff[i] == '>'){
			signe--;
		}
		i++;
	}
	if(strcmp(buff,"q/")==0){
		return 0;
	}
	if(signe != 0){
		return 1;
	}
	if(strcmp(instruction,"forward")!=0 && strcmp(instruction,"backward")!=0 && strcmp(instruction,"cw")!=0 && strcmp(instruction,"ccw")!=0){
		if(nombre_valeur < 1){
			return 1;
		}
	}
	if(strcmp(instruction,"set_v")==0){
		if(nombre_valeur < 2){
			return 1;
		}
	}
	return 0;
}
























