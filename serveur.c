/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <curses.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>



#define SERVICE_DEFAUT "1111"
#define PORT 1234

void serveur_appli (char *service);   /* programme serveur */


/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */

	srand(time(NULL));

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
	{
		case 1:
			printf("defaut service = %s\n", service);
			break;
		case 2:
			service=argv[1];
			break;

		default :
			printf("Usage:serveur service (nom ou port) \n");
			exit(1);
	}

	/* service est le service (ou num�ro de port) auquel sera affect�
	ce serveur*/

	serveur_appli(service);
}


/******************************************************************************/

/* peut etre utiliser pour parser les commandes */
int compare_cmd(char *cmd, char *client, int n, int *bien_place, int *mal_place){

	char lettre;
	int compt = 1;
	for (int i = 0; i < n; ++i){
		lettre = client[i];
		if(client[i] == cmd[i]){
			compt++;
		}
		else{
			return 0;
		}
	}
	return 1;
}

void serveur_appli(char *service)

/* Procedure correspondant au traitemnt du serveur de votre application */
{
    int socket_serv = socket(AF_INET,SOCK_STREAM,0); //création de la socket

    struct sockaddr_in ssin = { 0 }; /* initialise la structure avec des 0 */

    const char * hostname = service;

    ssin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

    ssin.sin_family = AF_INET;

    ssin.sin_port = htons(PORT);

    printf("Check 1");

    bind (socket_serv, (struct sockaddr *) &ssin, sizeof ssin);

    printf("Serveur : Bind fait");

    listen(socket_serv,5);

    printf("Serveur : Listen fait");

    struct sockaddr_in csin = { 0 };
    int csock;

    int sinsize = sizeof csin;


    csock = accept(socket_serv, (struct sockaddr *)&csin, &sinsize);

    printf("Connexion");

    char buffer[1024];
    int n = 0;

    n = recv(socket_serv, buffer, sizeof buffer - 1, 0);

    buffer[n] = '\0';

    printf("\n%d char reçus",n);

    printf("\n\n\n%s\n\n\n",buffer);


/*	char *cmd_quit = "quit";
	int num_socket = _socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in *padr_locale;
	adr_socket (service, NULL,SOCK_STREAM,&padr_locale);
	h_bind(num_socket,padr_locale);
	h_listen(num_socket,1);
	int num_socket_client = h_accept(num_socket,TOUT_CLIENT);
	printf("Connecté !\n");
	while (1) {

	}
/*	h_close(num_socket_client);
	h_close(num_socket);*/
}

/* fin des echanges avec le client */

/******************************************************************************/
