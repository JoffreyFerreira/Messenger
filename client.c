/******************************************************************************/
/*			Application: Messenger					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : FERREIRA Joffrey & OZENDA Thomas 					*/
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'�cran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/types.h> /* Consultez NOTES */
#include <sys/socket.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <string.h>


#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

typedef struct in_addr IN_ADDR;

#define PORT 1234

void client_appli (char *serveur, char *service);

/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
		case 1 :		/* arguments par defaut */
		printf("serveur par defaut: %s\n",serveur);
		printf("service par defaut: %s\n",service);
		break;
		case 2 :		/* serveur renseigne  */
		serveur=argv[1];
		printf("service par defaut: %s\n",service);
		break;
		case 3 :		/* serveur, service renseignes */
		serveur=argv[1];
		service=argv[2];
		break;
		default:
		printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */

	client_appli(serveur,service);
}

/*****************************************************************************/
void client_appli (char* serveur, char* service)
/* procedure correspondant au traitement du client de votre application */
{
	int socketClient = socket(AF_INET,SOCK_STREAM,0);

	struct hostent *hostinfo = gethostbyname(serveur);

	struct sockaddr_in ssin = { 0 }; /* initialise la structure avec des 0 */

	//&ssin = malloc(sizeof(struct sockaddr_in));

	//ssin.sin_addr = * (struct in_addr *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */

    ssin.sin_addr = *(struct in_addr *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */

	ssin.sin_port = htons(PORT); /* on utilise htons pour le port */
	ssin.sin_family = AF_INET;

	printf("Prêt à se connecter");

	connect(socketClient,(struct sockaddr *) &ssin, sizeof(struct sockaddr));

	printf("Connexion : succès");


	char buffer = 'l';

	send(socketClient, &buffer, 1, 0);

    /*
	int num_socket = h_socket(AF_INET,SOCK_STREAM);
	struct sockaddr_in *padr_distante;
	adr_socket (service,serveur,SOCK_STREAM,&padr_distante);
	h_connect(num_socket,padr_distante);
	printf("Connecté !\n");
	int gagne=0;
	while (gagne!=1) {
}
*/
}
/*****************************************************************************/
