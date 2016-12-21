#include "fonctions.h"

void creerFifo(char* nom){  // créé un fifo nommé "nom", avec les permissions de type "0666"
	if(nom == NULL){
		perror("Erreur de nom");
	}
	if (mkfifo(nom, 0666)== -1){
		perror("Erreur de création du tube");
	}
	return;
}

Node* creerNode(char* nom){ //Crée un noeud de nommé "nom", avec 0 ports
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
	
	
void supprimerNode(Node* noeud){ //Libère la mémoire allouée pour un noeud
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

void usageSimu(){ //fonction qui affiche les instructions pour le lancement du node_simu
	printf("./node_simu --map fichier_carte.bmp --init-pos x_initial y_initial --listen inport nomfifo.fifo --publish bumper_r nomfifo.fifo --publish bumper_l nomfifo.fifo\n");
	return;
}

void copieChaine(char* chaine_src, char* chaine_dest){ //Fonction qui à partir d'un nom de fichier "chaine_src" à la racine du projet, stoce dans "chaine_dest", le chemin menant à ce fichier depuis le lieu où est l'exécutable
	strcat(chaine_dest, "../");
	strcat(chaine_dest, chaine_src);
}

int testSimu(int argc, char** argv, char* listen, char* publish1, char* publish2, char* carte, float* x, float* y, Node* noeud){ //fonction qui lit les arguments entrés par l'utilisateur et remplit les champs correspondants, listen (nom du fifo écoute), publish1 (nom du fifo vers lequel on veut publier les informations d'un des bumpers), publish2 (nom du fifo vers lequel on veut publier les informations de l'autre bumper, carte (carte choisie), x et y (coordonnées du robot au début), noeud (nom du noeud)
	int i=1, cpt = 0;
	while(i<argc){
		if(strcmp(argv[i],"--publish")==0){
			if(strcmp(argv[i+1], "bumper_l")==0){
				publish1 = realloc(publish1, (strlen(argv[i+2])+3)*sizeof(char));
				copieChaine(argv[i+2], publish1);
				if(ajouterPort(noeud, 1, argv[i+1])==-1){return -1;}
				cpt++;
		
			}
			if(strcmp(argv[i+1], "bumper_r")==0){
				publish2 = realloc(publish2, (strlen(argv[i+2])+3)*sizeof(char));
				copieChaine(argv[i+2], publish2);
				if(ajouterPort(noeud, 1, argv[i+1])==-1){return -1;}
				cpt++;
			}
			i = i+3;
		}else{
			if(strcmp(argv[i],"--listen")==0){	
				listen = realloc(listen, (strlen(argv[i+2])+3)*sizeof(char));
				copieChaine(argv[i+2], listen);
				if(ajouterPort(noeud, 0, argv[i+1])){return -1;}
				i = i+3;
			}else{
				if(strcmp(argv[i],"--map")==0){ 
					carte = realloc(carte, (strlen(argv[i+1])+3)*sizeof(char));
					copieChaine(argv[i+1], carte);
					i = i+2;
				}else{
					if(strcmp(argv[i],"--init-pos")==0){
						*x = atof(argv[i+1]);
						*y = atof(argv[i+2]);
						i = i+3;
					}else{
						return -1;
					}
				}
			}
		}
	}
	return cpt;
}

int lectureInstruction(char* buff, char* instruction, char* valeur1, char* valeur2){ //fonction qui déduit du buffer l'instruction et les vitesses entrées par l'utilisateur
	int testInstruction = 0, i=0;
	while(buff[i]!='\n' && buff[i]!='\0' && buff[i]!=' '){
			if(testInstruction == 0){ 
				remplissageInstruction(buff, instruction, &i, &testInstruction);
			}else{
				if(testInstruction == 1){ 
					remplissageValeur1(buff, valeur1, &i, &testInstruction);
				}else{
					if(testInstruction == 2){ 
						remplissageValeur2(buff, valeur2, &i, &testInstruction);
					}
				}
			}
	}
	if(buff[i+1] == '\0'){
		return 1;
	}else{
		return 0;
	}
}

void remplissageInstruction(char* buff, char* instruction, int*i, int* testInstruction){ //Remplit dans la chaîne "instruction" l'instruction lue à partir du buffer qui provient de la lecture du fifo
	if(buff[*i]!='/'){
		instruction[strlen(instruction)] = buff[*i];
		(*i)++;
	}else{
			*testInstruction = 1;
			(*i)++;
	}
}

void remplissageValeur1(char* buff, char* valeur1, int*i, int* testInstruction){//Remplit dans la chaîne "valeur1" la première valeur lue à partir du buffer qui provient de la lecture du fifo
	if(buff[*i]!='/'){
		valeur1[strlen(valeur1)] = buff[*i];
		(*i)++;
	}else{
		*testInstruction = 2;
		(*i)++;
	}
}

void remplissageValeur2(char* buff, char* valeur2, int*i, int* testInstruction){ //Remplit dans la chaîne "valeur2" la deuxième valeur potentiellement lue à partir du buffer qui provient de la lecture du fifo
	if(buff[*i]!='/'){
		valeur2[strlen(valeur2)] = buff[*i];
		(*i)++;
	}else{
			*testInstruction = 0;
			(*i)++;
	}
}


void lectureSimu(Simu* arg1){ //Fonction qui à partir d'une variable de type Simu*, va comprendre les instructions, bouger et afficher le robot en conséquence

	//Initialisations
	char *instruction, *valeur1, *valeur2;
	instruction = malloc(BUFFSIZE*sizeof(char));memset(instruction,'\0', BUFFSIZE);
	valeur1 = malloc(BUFFSIZE*sizeof(char));memset(valeur1,'\0', BUFFSIZE);
	valeur2 = malloc(BUFFSIZE*sizeof(char));memset(valeur2,'\0', BUFFSIZE);	
	float vitesse1;
	float vitesse2;

	//Mise à jour des vitesses du robot
	lectureInstruction(arg1->buff, instruction, valeur1, valeur2); //Lecture des instructions à partir du buffer
	vitesse1 = atof(valeur1); //Conversion des valeurs de vitesse de chaînes de caractères en flottants
	vitesse2 = atof(valeur2);
	inversion(instruction,&vitesse1, &vitesse2);
	ordreSimu(instruction, vitesse1, vitesse2, arg1->vr, arg1->vl); //Extraction des vitesses de chaque moteur en fonction des instructions et vitesses désirées

	//Appels généraux pour les bumpers, déplacement et affichage du robot
	arg1->bumper_l = get_left_bumper(arg1->robot); //extraction des informations des bumpers
	arg1->bumper_r = get_right_bumper(arg1->robot);
	move_robot(arg1->robot, arg1->carte, *(arg1->vr), *(arg1->vl));
	update_display(arg1->affichage);
	usleep(500); //Pause pour que l'utilisateur puisse voir les mouvements du robot

	free(instruction);free(valeur1);free(valeur2); //Libération des pointeurs utilisés uniquement au sein de la fonction
}

int ordreSimu(char* instruction, float vitesse1, float vitesse2, float* vr, float* vl){ //Fonction qui lit l'instruction et les vitesses entrées par l'utilisateur et en déduit les vitesses de chaque moteur
	if(strcmp(instruction,"forward")==0){
		*vr = vitesse1;
		*vl = vitesse1;
	}else{
		if(strcmp(instruction,"backward")==0){
			*vr = -vitesse1;
			*vl = -vitesse1;		
		}else{
			if(strcmp(instruction,"cw")==0){
				*vr = vitesse1;
				*vl = -vitesse1;
			}else{
				if(strcmp(instruction,"ccw")==0){
					*vr = -vitesse1;
					*vl = vitesse1;
				}else{
					if(strcmp(instruction,"stop")==0){
						*vr = 0;
						*vl = 0;
					}else{
						if(strcmp(instruction,"set_v")==0){
							*vr = vitesse1;
							*vl = vitesse2;
						}else{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}


void inversion(char* instruction, float* valeur1, float* valeur2){ //fonction qui inverse les signes des vitesses pour les commandes forward, backward, cw et ccw si ceux-ci sont négatifs
	if(strcmp(instruction,"forward")==0 || strcmp(instruction,"backward")==0 || strcmp(instruction,"cw")==0 || strcmp(instruction,"ccw")==0){
		if(*valeur1<0.0){
			*valeur1 = -*valeur1;
		}
		if(*valeur2<0.0){
			*valeur2 = -*valeur2;
		}
	}
}				
				































