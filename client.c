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

    ssin.sin_addr = *(struct in_addr *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	ssin.sin_port = htons(PORT); /* on utilise htons pour le port */
	ssin.sin_family = AF_INET;

	printf("Prêt à se connecter\n");

	connect(socketClient,(struct sockaddr *) &ssin, sizeof(struct sockaddr));

	printf("Connexion : succès\n");

	printf("Veuillez rentrer votre pseudo :");

	char buffer[25] = "";

    fgets(buffer, sizeof(chaine), stdin);
    clean(chaine, stdin);

    fd_set rdfs;

	while(1){
        FD_ZERO(&rdfs);
    }

}

void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}
/*****************************************************************************/
