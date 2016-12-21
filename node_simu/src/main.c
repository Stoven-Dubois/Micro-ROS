#include "main.h"
#include "fonctions.h"


void* continuation_mouvement(void* arg){ //Thread qui continue de faire bouger et d'afficher le robot tant qu'aucune nouvelle commande n'est entrée par l'utilisateur
	Simu* arg1 = (Simu*)arg;
	
	//Tant que l'utilisateur n'a entré aucune nouvelle commande
	while(arg1->test_lecture == 0){
		move_robot(arg1->robot, arg1->carte, *(arg1->vr), *(arg1->vl));
		update_display(arg1->affichage);
		usleep(500000);
	}
	pthread_exit(NULL);
}

void* lecture(void * arg){ //Thread qui va s'arrêter quand l'utilisateur entre une nouvelle commande
	Simu* arg1 = (Simu*)arg;
	memset(arg1->buff, '\0', BUFFSIZE);
	read(arg1->fd, arg1->buff, BUFFSIZE);//Lecture du fifo
	
	
	if(strcmp(arg1->buff,"q/-1")==0){
		arg1->test = 1;
	}
	arg1->test_lecture = 1;
	pthread_exit(NULL);
}

int main(int argc, char** argv){
	//Test du nombre d'arguments
	if((argc != 9)&&(argc!=12)&&(argc!=15)){
		usageSimu();
		return -1;
	}

	//Initialisations
	int mode, bumper_l = 0, bumper_r = 0;
	float x, y, vr=0, vl=0;
	char* listen=malloc(BUFFSIZE*sizeof(char));
	char* publish1=malloc(BUFFSIZE*sizeof(char)); 
	char* publish2=malloc(BUFFSIZE*sizeof(char));
	char* carte=malloc(BUFFSIZE*sizeof(char)); //Chaînes contenant les noms des fichiers entrés en argument par l'utilisateur
	Node* nodeSimu = creerNode(argv[0]);

	//Lecture des arguments
	mode = testSimu(argc, argv, listen, publish1, publish2, carte, &x, &y, nodeSimu);
	mode = 2;
	if(mode==-1){printf("Erreur lors de la lecture des arguments\n");}

	//Initialisations des variables liées au robot
	robot_t* robot = new_robot(x, y);
	map_t* map = new_map(carte);
	display_t* display = new_display(map, robot);
	
	//Ouverture des fifos pour publier les informations sur les bumpers
	int f1 = -1, f2 = -1;

	//Buffer de reception
  	char *buff = malloc(BUFFSIZE*sizeof(char));
  	memset(buff, '\0', BUFFSIZE);
	
	// Lecture de la chaine dans le pipe
	int fd = open(listen, O_RDONLY);

	//Initialisation de la variable de type Simu contenant toutes les informations contenant le robot	
	Simu arg1 = {buff,fd,bumper_r,bumper_l,f1,f2,robot,map,display,&vr,&vl,0,0};

	// Création des threads de lecture et de vérification d'interruption
	pthread_t p_continuation_mouvement, p_lecture;
	while(arg1.test == 0){
		if((pthread_create(&p_continuation_mouvement, NULL, &continuation_mouvement, (void*)&arg1)) != 0){
			perror("Erreur de création du thread 1");
		}
		if((pthread_create(&p_lecture, NULL, &lecture, (void*)&arg1)) != 0){
			perror("Erreur de création du thread 1");
		}
		// Attente mutuelle, les deux threads s'arrêtent quand l'utilisateur entre une nouvelle commande
		if((pthread_join(p_lecture, NULL)) != 0){
			perror("Erreur d'interraction des threads");
		}
		if((pthread_join(p_continuation_mouvement, NULL)) != 0){
			perror("Erreur d'interraction des threads");
		}
		lectureSimu(&arg1); //Compréhension de la nouvelle commande entrée par l'utilisateur
		arg1.test_lecture = 0;
	}
  	

  	// Nettoyage
	free(listen);
	free(carte);
	supprimerNode(nodeSimu);
	printf("fin nodeSimu\n");
	return 0;
}
	
