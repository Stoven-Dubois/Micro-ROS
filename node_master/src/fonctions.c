#include "fonctions.h"

Cfg** creerCfg(char* path, int* nb){ //fonction qui va lire le fichier de configuration dont le chemin est path, remplir nb avec le nombre de noeuds et retourner la variable de type Cfg
	
	//Déclarations et initialisations
	*nb = 0;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	char* indic;
	Cfg** config = malloc(BUFFSIZE*sizeof(Cfg*));
	if( (config = malloc((*nb)*sizeof(Cfg*))) == NULL ){
		perror("Erreur d'allocation de la config");
		return NULL;
	}
	config[*nb] = malloc(sizeof(Cfg));
	
	//Déclaration et initialisation de la variable de type VeriFifo** pour vérifier les fifos
	VeriFifo** verififo;
	int nbFifo = 0;
	if((verififo = malloc((nbFifo)*sizeof(VeriFifo*))) == NULL ){
		perror("Erreur d'allocation de la config");
		return NULL;
	}
	
	// Ouverture du fichier de configuration
	FILE* fp;
	fp = fopen(path, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		indic = getIndicateur(line);
		if(strcmp(indic,"node") == 0){
			remplissageNoeud(nb, config, line);
		}
		else if( strcmp(indic,"  bin") == 0){
			remplissageBin(nb, config, line);
		}
		else if(strcmp(indic,"  publish") == 0){
			remplissagePublish(nb, config, line);
			if(constructionFifo(verififo, (config[*nb])->publish,nb, &nbFifo, 1)==1){
				return NULL;
			}
		}
		else if(strcmp(indic,"  listen") == 0){
			remplissageListen(nb, config, line);
			if(constructionFifo(verififo, (config[*nb])->listen,nb, &nbFifo, 0)){
				return NULL;
			}
		}
		else if(strcmp(indic,"  args") == 0){
			remplissageArgs(nb, config, line);
		}
		else{
			printf("Ligne incorrecte\n");
		}
	}
	//Libérations
	if (line)
		free(line);
	if(testFifo(verififo, nbFifo)==1){
		config =  NULL;
	}
	return config;
}

void remplissageNoeud(int* nb, Cfg** config, char* line){ //Fonction qui stocke le nouveau noeud dans la variable config
	// Creation d'une variable Cfg* pour stocker un nouveau noeud
	(*nb)++;
	config[*nb] = malloc(sizeof(Cfg));
	// Remplissage
	(config[*nb])->noeud = getArgs(line);
}

void remplissageBin(int* nb, Cfg** config, char* line){  //Fonction qui stocke la composante bin du noeud dans la variable config
	//Remplissage du bin dans config
	(config[*nb])->bin = getArgs(line);
	//Verification d'existence
	existCheck((config[*nb])->bin);
}

void remplissagePublish(int* nb, Cfg** config, char* line){  //Fonction qui stocke la composante publish dans la variable config
	//Remplissage du publish dans config
	(config[*nb])->publish = getArgs(line);
}

void remplissageListen(int* nb, Cfg** config, char* line){  //Fonction qui stocke la composante listen dans la variable config
	//Remplissage du listen dans config
	(config[*nb])->listen = getArgs(line);
}

void remplissageArgs(int* nb, Cfg** config, char* line){  //Fonction qui stocke les arguments dans la variable config
	//Remplissage des arguments dans config
	(config[*nb])->args = getArgs(line);
}


void supprimerCfg(Cfg** cfg, int nb){ //fonction qui libère la mémoire allouée pour une variable de type Cfg**
	int i;
	for(i=0;i<nb;i++){
		if((cfg[i]->noeud)!=NULL)
			free(cfg[i]->noeud);
		if((cfg[i]->bin)!=NULL)
			free(cfg[i]->bin);
		if((cfg[i]->publish)!=NULL)
			free(cfg[i]->publish);
		if((cfg[i]->listen)!=NULL)
			free(cfg[i]->listen);
		if((cfg[i]->args)!=NULL)
			free(cfg[i]->args);
		if((cfg[i])!=NULL)
			free(cfg[i]);
	}
	if(cfg!=NULL)
		free(cfg);
}

char* getIndicateur(char *str) { //fonction qui retourne le mot débutant une ligne dans le fichier de configuration
	int i = 0; 
	char *stock; 
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	while (str[i]) {  
		if (str[i] == ':') { 
			break;
		} 
		stock[i] = str[i]; 
		i++;
	} 
	stock[i] = '\0'; 
	return stock; 
}


char* getArgs(char *str) { //fonction qui retourne les arguments situés après les deux points dans une ligne du fichier de configuration
	int i = 0, j = 0; 
	char *stock;
	int boolean = 0;
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	memset(stock, 0, sizeof(char) * (strlen(str) + 1));
	while (str[i]) {  
		if(boolean == 1){
			if (str[i] == '\n') { 
				break;
			} 
			stock[i-j-1] = str[i];
		}
		if (str[i] == ':') { 
			boolean = 1;
			i++;
			j = i;
		}		
		i++;
	} 
	stock[i] = '\0'; 
	return stock; 
}

char** parse(char* str){  //fonction qui transforme une chaîne de caractères en tableau de chaînes de caractères avec un mot par case de tableau
	int i = 0, j = 0, k = 0, offset = 0, cpt = 1;
	while(str[i]){
		if(str[i] == ' '){
			cpt++;
		}
		i++;
	}
	i = 0;
	char** stock;
	stock = calloc(cpt,sizeof(char*));
	for(j=0;j<cpt;j++){
		stock[j] = calloc(BUFFSIZE,sizeof(char));
		memset(stock[j],'\0',BUFFSIZE);
	}
	while (str[i]) {  
		if ((str[i] == ' ') || (str[i] == '\0') ) {
			stock[k][i] = '\0';
			k++;
			memset(stock[k],'\0',500);
			i++;
			offset = i;
					
		} 
		stock[k][i-offset] = str[i]; 
		i++;
	}
	stock[k][i-offset+1] = '\0';
	stock[cpt] = NULL;
	return stock; 
}

int hidenchar(char *str, char c){ //fonction qui repère la case d'une chaîne de caractères "str" où le symbole "c" est présent
	int i;	
	i = 0;
	while (str[i] != c && str[i] != '\0')
  		i++;
	if (str[i] == '\0')
	  	return (1);
  	return (0);
}

char **my_comp_tab(char *mem, char **tab, char *space){ //fonction qui ajoute des '\0' pour finir les chaînes de caractères d'un tableau de chaînes de caractère
	int n, i;
	n = 0;
	i = 0;
	tab[n] = &mem[i];
	while (mem[i] != '\0'){
      		if (hidenchar(space, mem[i]) == 0){
	  		if (mem[i] != '\0')
	    			mem[i++] = '\0';
	 		while (hidenchar(space, mem[i]) == 0)
	    			i++;
	  		if (mem[i] != '\0')
	    			tab[++n] = &mem[i];
		}
      		if (mem[i] != '\0')
			i++;
    	}
 	tab[n + 1] = 0;
  	return (tab);
}

char **my_str_to_wordtab(char *str, char *space){ //fonction qui retourne un tableau de chaînes de caractères bien finies par '\0'
	int i;
 	char **tab;
  	char *mem;

  	i = strlen(str);
  	mem = strdup(str);
  	if ((tab = malloc(sizeof(*tab) * i + 1)) != NULL){
      		tab[0] = 0;
     		tab = my_comp_tab(mem, tab, space);
    	}else
    		exit(EXIT_FAILURE);
  	return (tab);
}

char* getArg1(char *str) { //fonction qui retourne le mot situé à gauche de la flèche dans le fichier de configuration
	int i = 0; 
	char *stock; 
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	while (str[i]) {  
		if (((str[i] == '-') && (str[i+1] == '>')) || ((str[i] == '<') && (str[i+1] == '-'))) { 
			break;
		} 
		stock[i] = str[i]; 
		i++;
	} 
	stock[i] = '\0'; 
	return stock; 
}

char* getArg2(char *str) {  //fonction qui retourne le nom du fifo à droite de la flèche dans le fichier de configuration
	int i = 0, j = 0; 
	char *stock;
	int boolean = 0;
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	memset(stock, 0, sizeof(char) * (strlen(str) + 1));
	while (str[i]) {  
		if(boolean == 1){
			if (str[i] == '\n') { 
				break;
			} 
			stock[i-j-1] = str[i];
		}
		if (((str[i] == '-') && (str[i+1] == '>')) || ((str[i] == '<') && (str[i+1] == '-'))) { 
			boolean = 1;
			i++;
			i++;
			j = i;
		}		
		i++;
	} 
	stock[i] = '\0'; 
	return stock; 
}

void prettyPrint(Cfg** config, int* nb){  //fonction qui effectue le pretty print
	int i = 1;
	printf("\n");
	for(i=1;i<=*nb;i++){
		printf("* %s [%s %s]\n", (config[i])->noeud, (config[i])->bin, (config[i])->args);
		printf("** publish :\n");
		if((config[i])->publish != NULL){
			printf("*** %s\n", (config[i])->publish);
		}
		printf("** listen :\n");	
		if((config[i])->listen != NULL){
			printf("*** %s\n",(config[i])->listen);
		}
		printf("\n");
	}
}

char* existCheck(char* binary){ //fonction qui vérifie que l'exécutable existe
	char* name = getBinName(binary);
	char* path = malloc((5+2*strlen(name))*sizeof(char));
	strcat(path, "../");
	strcat(path, name);
	strcat(path, "/");
	strcat(path, name);
	FILE* fp;
	if( (fp = fopen(path, "r")) == NULL){
		printf("%s doesn't exist\n", path);
		return ("error");
	}
	else{
		printf("%s exists\n", path);
		return path;
	}
}

char* getBinName(char *str){ //fonction qui extrait le nom du bin de la chaîne de caractères str 
	int i = 0, j = 0; 
	char *stock;
	int boolean = 0;
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	memset(stock,'\0', (strlen(str) + 1));
	while (str[i]) {  
		if(boolean == 1){
			if (str[i] == '\n') { 
				break;
			} 
			stock[i-j-1] = str[i];
		}
		if (str[i] == '/') { 
			boolean = 1;
			j = i;
		}		
		i++;
	} 
	stock[i] = '\0'; 
	return stock; 
}


char* getFifo(char* str, int mode){ //fonction qui extrait le nom du fifo de la chaîne de caractères str, avec mode = 0, si on est en listen et mode = 1 si on est en publish
	int i = 0, j = 0; 
	char *stock;
	int boolean = 0;
	char fleche;
	if(mode == 0){
		fleche = '<';
	}
	if(mode==1){
		fleche = '>';
	}
	stock = malloc(sizeof(char) * (strlen(str) + 1)); 
	while (str[i]) {  
		if(boolean == 1){
			if (str[i] == '\n') { 
				break;
			} 
			stock[i-j-1] = str[i];
		}
		if (str[i] == fleche) {
			boolean = 1;
			if(mode==0){
				i=i+2;
			}else{
				i=i+1;
			}
			j = i;
		}	
		i++;
	} 
	if(boolean == 0){
		printf("Erreur d'écriture de sens pour la flèche précédent le fifo\n");
		return("erreur");
	}
	stock[i] = '\0'; 
	return stock; 
}

int majFifo(VeriFifo** verififo, char* nom, int nb, int mode){//Fonction qui regarde si le fifo a deja ete rencontre et qui modifie l'entier "verifie" en conséquence
	int i;
	for(i=1;i<=(nb);i++){
		if(strcmp(verififo[i]->nom,nom)==0){
			if(verififo[i]->mode != mode){
				verififo[i]->verifie = 1;
				return 0;
			}else{
				return 0;
			}
		}
	}
	return 1;
}

int testFifo(VeriFifo** verififo, int nb){ //fonctio qui, à partir d'une variable de type Verififo** remplie, dit si les fifos sont bien écrites dans le fichier de configuration
	int i, test = 1;
	for(i=1;i<=(nb);i++){
		if(verififo[i]->verifie == 0){
			test = 0;
		}
		free(verififo[i]);
	}
	if(test == 0){
		printf("Erreur d'écriture des fifo\n");
		return 1;
	}
	return 0;
}

int constructionFifo(VeriFifo** verififo, char* chaineMode,int* nbLecture, int* nbFifo, int mode){ //fonction qui va remplir la variable de type Verififo** tout au long de la lecture du fichier de configuration
	char* nomFifo = getFifo(chaineMode,mode);
	if(strcmp(nomFifo, "erreur")==0){
		return 1;
	}
	if(majFifo(verififo,getFifo(chaineMode,mode),(*nbLecture)-1,mode)==1){
		(*nbFifo)++;
		verififo = realloc(verififo, (*nbFifo)*sizeof(Cfg*));
		verififo[*nbFifo] = malloc(sizeof(VeriFifo));
		verififo[*nbFifo]->nom = nomFifo;
		verififo[*nbFifo]->mode = mode;
		verififo[*nbFifo]->verifie = 0;
	}
	return 0;
}

void creerFifo(char* nom){  // créé un fifo nommé "nom", avec les permissions de type "0666"
	if(nom == NULL){
		perror("Erreur de nom");
	}
	if (mkfifo(nom, 0666)== -1){
		perror("Erreur de création du tube");
	}
	return;
}


void supprimerVeriFifo(VeriFifo** verififo, int nb){ //libère la mémoire allouée pour une variable de type VeriFifo**
	int i;
	for(i=0;i<nb;i++){
		free(verififo[i]->nom);
		free(verififo[i]);
	}
	free(verififo);
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

int ajouterPortEntree(Node* noeud, char* nom){ //fonction qui ajoute un port d'entrée "nom" au noeud "noeud"
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

int ajouterPortSortie(Node* noeud, char* nom){  //fonction qui ajoute un port de sortie "nom" au noeud "noeud"
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

void usageMaster(){  //fonction qui affiche les instructions pour le lancement du node_logger
	printf("./node_master myconfig.txt \n");
	return;
}

void copieChaine(char* chaine_src, char* chaine_dest){   //fonction qui à partir d'un nom de fichier "chaine_src" à la racine du projet, stocke dans "chaine_dest", le chemin menant à ce fichier depuis le lieu où est l'exécutable
	strcat(chaine_dest, "../");
	strcat(chaine_dest, chaine_src);
}
