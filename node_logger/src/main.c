#include "main.h"

void* lecture(void* arg){ //Fonction qui va lire le fifo, écrire dans le fichier log et publié, si spécifié par l'utilisateur, dans un autre fifo
	Logger* arg1 = (Logger*)arg;
	while(arg1->test == 0){
		read(arg1->fd, arg1->buff, BUFFSIZE); //On lit le message dans le fifo
 		fseek(arg1->log, 0, SEEK_END);	
  		fprintf(arg1->log, "%s", arg1->buff); //On l'écrit dans le fichier "log"
		if(arg1->fdPublish!=-1){
			write(arg1->fdPublish, arg1->buff, BUFFSIZE); //Si voulu, on publit dans un autre fifo
		}
		memset(arg1->buff,'\0',BUFFSIZE);
	}
	pthread_exit(NULL);
}

void* testoff(void* arg){ //fonction qui va regarder si l'utilisateur entre la touche permettant d'arrêter le programme
	Logger* arg1 = (Logger*)arg;
	int fd = open("../gestion_scanf.fifo",O_RDONLY);  //On ouvre le fifo contenant les informations du scan
	char* buff = malloc(BUFFSIZE*sizeof(char));
	while(arg1->test == 0){ 
		read(fd, buff, BUFFSIZE);
		if(strcmp(buff,"q<-1>")==0){  //On regarde si l'utilisateur appuie sur la touche "q<-1>"
				arg1->test = 1;
		} 
	}
	close(fd);
	free(buff);
	pthread_exit(NULL);
}

int main(int argc, char** argv){
	if((argc != 6)&&(argc!=9)){  //Affichage des instructions s'il n'y a pas le bon nombre d'arguments
		usageLogger();
		return -1;
	}
	
	//Initialisations
	char* listen=malloc(BUFFSIZE*sizeof(char)),* log=malloc(BUFFSIZE*sizeof(char)),* publish=malloc(BUFFSIZE*sizeof(char));
	Node* nodeLogger = creerNode(argv[0]);

	//Lecture des arguments du main
	if((testLogger(argc, argv, listen, log, publish, nodeLogger))!=0){printf("Erreur lors de la lecture des arguments\n");}
	
	// Ouverture du fifo
	int fd = open(listen, O_RDONLY);

	//Déclaration et initialisation du buffer de reception
  	char* buff = malloc(BUFFSIZE*sizeof(char));
  	memset(buff, '\0', BUFFSIZE);
	
	//Ouverture du fichier log
	remove(log);
	FILE* log_file = fopen(log,"w");
	
	//Initialisation de la variable de type Logger
	Logger arg1 = {buff,fd,log_file,0,-1};
	
	//Si l'utilisateur veut publier dans un fifo
	if(argc == 9){
		creerFifo(publish);
		int fd = open(publish, O_WRONLY);
		arg1.fdPublish = fd;
	}
	
	// Création des threads de lecture et de vérification d'interruption
	pthread_t t1, t2;
	if((pthread_create(&t1, NULL, &lecture, (void*)&arg1)) != 0){
		perror("Erreur de création du thread 1");
		return -1;
	}
	if((pthread_create(&t2, NULL, &testoff, (void*)&arg1)) != 0){
		perror("Erreur de création du thread 2");
		return -1;
	}
	
	// Attente mutuelle
	if((pthread_join(t2, NULL)) != 0){ //Si le thread testOff finit, les deux threads s'arrêtent
		perror("Erreur d'interraction des threads");
		return -1;
	}
  	
  	// Nettoyage
	supprimerNode(nodeLogger);
	supprimerLogger(&arg1);
	free(listen);
	free(publish);
	printf("nodeLogger terminé\n");
	return 0;
}
