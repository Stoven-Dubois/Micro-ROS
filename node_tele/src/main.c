#include "main.h"

int main(int argc, char** argv){
	usageTele(argc);

	//Initialisations
	Node* nodeTele = creerNode(argv[0]);
	char* publish = malloc(BUFFSIZE*sizeof(char));
	char* instruction = malloc(BUFFSIZE*sizeof(char));memset(instruction,'\0', BUFFSIZE);
	char* buff = malloc(BUFFSIZE*sizeof(char));memset(buff,'\0', BUFFSIZE);
	char* buff2 = malloc(BUFFSIZE*sizeof(char));memset(buff2,'\0', BUFFSIZE);
	char* valeur1 = malloc(BUFFSIZE*sizeof(char));memset(valeur1,'\0', BUFFSIZE);
	char* valeur2 = malloc(BUFFSIZE*sizeof(char));memset(valeur2,'\0', BUFFSIZE);

	
	if(testTele(argc, argv, publish, nodeTele)!=0){
		return -1;
	}


	// Ouverture du fifo du scanf et du fifo vers lequel on va publier
	int fd = open(publish, O_WRONLY);
	int fp = open("../gestion_scanf.fifo", O_RDONLY);
	int i, test = 0;
	
	//Lecture des instructions envoyées par l'utilisateur et écriture dans le fifo
	while(strcmp(buff,"q<-1>")!=0){
		test = 0;
		while(test == 0){
			memset(buff2,'\0', BUFFSIZE);
			memset(buff,'\0',BUFFSIZE);
			read(fp, buff, BUFFSIZE); //on stocke dans buff les commandes envoyées par l'utilisateur
			memset(instruction,'\0', BUFFSIZE);
			i = 0;
			while(buff[i]!= '<' && buff[i]!='\0'){
				instruction[i] = buff[i];
				i++;
			}
			strcat(buff2,instruction); //buff2 contient la chaîne à envoyer au fifo
			strcat(buff2,"/");
			test = 1;

			//On regarde si la commande entrée par l'utilisateur est conforme
			if(test_entree(buff, instruction)!=0){
				printf("Instruction invalide, recommencez :\n");
				test = 0;
			}
		}	
		i++;
		

		//lecture des valeurs entrées par l'utilisateur
		while(buff[i]!='>' && buff[i]!='\0'){
			valeur1[i-(strlen(instruction)+1)] = buff[i];
			i++;
		}
		strcat(buff2,valeur1);
		i++;
		if(strcmp(instruction,"set_v")==0){
			while(buff[i]!='>' && buff[i]!='\0'){
				valeur2[i-(+strlen(instruction)+strlen(valeur1)+3)] = buff[i];
				i++;
			}
			i++;
			strcat(buff2,"/");
			strcat(buff2,valeur2);
		}
		write(fd, buff2, BUFFSIZE);
		
		memset(valeur1,'\0', BUFFSIZE);
		memset(valeur2,'\0', BUFFSIZE);
		i=0;
	}
	printf("fin nodeEcho\n");

	//Libération
	supprimerNode(nodeTele);
	free(buff);
	free(buff2);
	free(instruction);
	return 0;
}
