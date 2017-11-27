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

#include<stdio.h>
#include <curses.h>

#include<sys/signal.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"

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
	char *cmd_quit = "quit";
	int num_socket = h_socket(AF_INET,SOCK_STREAM);
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
