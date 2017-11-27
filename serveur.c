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
	int num_socket = h_socket(AF_INET,SOCK_STREAM);
	struct sockaddr_in *padr_locale;
	adr_socket (service, NULL,SOCK_STREAM,&padr_locale);
	h_bind(num_socket,padr_locale);
	h_listen(num_socket,1);
	int num_socket_client = h_accept(num_socket,TOUT_CLIENT);

	/* partie echange avec le client */

	int taille = 1;
	char taille_n[1];
	int n = 0;
	int taille_effective = 0;
	taille_effective = h_reads(num_socket_client,taille_n,taille);
	printf("ok\n");
	n = 2*(atoi(taille_n))+1;

	// on envoie les consignes au client
	//printf("Message pret à envoyer\n");
	h_writes(num_socket_client, "Rentrez une chaine de caractere :\nR = rouge\nY = jaune\nG = vert\nB = bleu\nO = orange\nW = blanc\nV = violet", 110);
	//printf("Message envoyé\n");

	int bien_place;
	int mal_place;
	int gagne = 0;
	int nbr_coup = 1;
	char client[n];
	char retour[n];
	char attente[1];

	while(gagne!=1){
		printf("Au client de jouer\n");
		taille_effective = h_reads(num_socket_client, client, n); //on lit la chaine du client
		printf("Le client a saisi : ");
		for(int i=0;i<taille_effective;i++){
			printf("%c",client[i]);
		}
		printf("\n");
		bien_place = 0;
		mal_place = 0;
		printf("J'ai comparé, bien place : %d et mal place : %d\n",bien_place,mal_place);
		char tampon1[n];
		int lg_tmp1;
		if (bien_place>=10){
			lg_tmp1 = 2;
		}
		else{
			lg_tmp1 = 1;
		}
		char lgtmp1[1];
		sprintf(lgtmp1, "%d", lg_tmp1);
		int lg_tmp2;
		if (mal_place>=10){
			lg_tmp2 = 2;
		}
		else{
			lg_tmp2 = 1;
		}
		char lgtmp2[1];
		sprintf(lgtmp2, "%d", lg_tmp2);
		char tampon2[n];
		sprintf(tampon1, "%d", bien_place);
		sprintf(tampon2, "%d", mal_place);
		printf("\n");
		h_writes(num_socket_client, lgtmp1, 1);
		taille_effective = h_reads(num_socket_client, attente, 1);
		h_writes (num_socket_client, tampon1, lg_tmp1) ; // on envoie
		taille_effective = h_reads(num_socket_client, attente, 1); // ne pas ecraser avec le 2eme write la valeur envoyé par le 1er
		h_writes(num_socket_client, lgtmp2, 1);
		taille_effective = h_reads(num_socket_client, attente, 1);
		h_writes (num_socket_client, tampon2, lg_tmp2) ;
		char trans[1];
		sprintf(trans, "%d", gagne);
		h_writes(num_socket_client,trans,1);
		h_close(num_socket_client);
		h_close(num_socket);
	}

}

/* fin des echanges avec le client */

/******************************************************************************/
