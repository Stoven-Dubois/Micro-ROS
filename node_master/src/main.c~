#include "main.h"

void exec(char* path, char** commande, char** env){
	pid_t p = fork();
	if(p == 0){
		execve(path,commande,env);
	}
}


int main(int argc, char** argv, char** env){
	if(argc!=2){
		usageMaster();
		return 0;
	}
	int nb; // nombre de nodes qui sera renseigné en lisant la config
	Cfg** config = creerCfg(argv[1], &nb);
	if((config==NULL)){
		usageMaster();
		return 0;
	}

	printf("****************************************************\n");
	printf("******************* PRETTY PRINT *******************\n");
	printf("****************************************************\n");
	prettyPrint(config, &nb);	
	printf("****************************************************\n");
	printf("****************************************************\n");
	
	// Initialisations
	int i;
	char cmd[MAXCOMMANDSIZE];
	memset(cmd,'\0',MAXCOMMANDSIZE);

	char* name;
	char* path;
	char* nomFifo = malloc(BUFFSIZE*sizeof(char));memset(nomFifo,'\0', BUFFSIZE);
	char**  commande;
	char chaine_lue[BUFFSIZE];
	

	//Pretty print et remplissage de commandes et chemins à exécuter
	for(i=nb;i>0;i--){
		name = getBinName((config[i])->bin);//path contient les chemins vers les exécutables
		path = calloc((6+2*strlen(name)),sizeof(char));
		strcat(path, "../");
		strcat(path, name);
		strcat(path, "/");
		strcat(path, name);
		
		memset(cmd,0,MAXCOMMANDSIZE); //on lit toutes les commandes depuis la variable "config"
		strcat(cmd, name);
		strcat(cmd, " ");

		
		if((config[i])->listen != NULL){
			strcat(cmd, "--listen ");
			strcat(cmd, getArg1((config[i])->listen));
			strcat(cmd, getArg2((config[i])->listen));
		}
		strcat(cmd," ");
		

		if((config[i])->publish != NULL){
			strcat(cmd, "--publish ");
			strcat(cmd, getArg1((config[i])->publish));
			strcat(cmd, getArg2((config[i])->publish));
			strcat(nomFifo, "../");
			strcat(nomFifo, getArg2((config[i])->publish));printf("nomfifo : %s\n", nomFifo);
			//Création des fifo
			remove(nomFifo);
			creerFifo(nomFifo);
		}
		strcat(cmd, " ");
		if(config[i]->args != NULL){
			strcat(cmd, (config[i])->args);
		}
		strcat(cmd, "");

		commande = my_str_to_wordtab(cmd," ");  //commande est un tableau de chaîne de caractères contenant les exécutables

		
		//On fork pour garder la main même après avoir lancé les exécutables
		int execution = fork();
		if(execution == 0){
			exec(path, commande, env);
		}else{

			//On va créer un fifo pour le scanf. Ce fifo est codé en dur dans le master car c'est un fifo général que tous les noeuds peuvent lire.
			printf("Pour quitter le programme, tapez \"q<-1>\"\n");
			remove("../gestion_scanf.fifo");
			creerFifo("../gestion_scanf.fifo");
			int f_scanf = open("../gestion_scanf.fifo", O_WRONLY);
			memset(chaine_lue,'\0', BUFFSIZE);
			while(strcmp(chaine_lue,"q<-1>")!=0){
				scanf("%s", chaine_lue);
				write(f_scanf, chaine_lue, BUFFSIZE); 
			}
			printf("Fin du master\n");
		}
		
	}
	//Libération
	supprimerCfg(config,nb);
	free(name);
	free(path);
	free(commande);
	return 0;
}
