/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'�cran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

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
void client_appli (char *serveur,char *service)
/* procedure correspondant au traitement du client de votre application */
{
int num_socket = h_socket(AF_INET,SOCK_STREAM);
struct sockaddr_in *padr_distante;
adr_socket (service,serveur,SOCK_STREAM,&padr_distante);
h_connect(num_socket,padr_distante);

// Rentrée par l'utilisateur de la difficultée //

printf("Quel niveau de difficulté ? : \n1 -> 3 couleurs \n2 -> 5 couleurs \n3 -> 7 couleurs \n4 -> 9 couleurs \n5 -> 11 couleurs \n6 -> 13 couleurs\n");
char niveau[1];
scanf("%s",niveau);
h_writes(num_socket,niveau,1);
int choice = niveau[0]-'0';
int messages_length = (choice*2)+1;

char message_from_serveur[110];

int taille_message_serveur = h_reads(num_socket,message_from_serveur,110);

for(int i=0;i<taille_message_serveur;i++){
	printf("%c",message_from_serveur[i]);
}

printf("\n");
int gagne=0;
while (gagne!=1) {
	printf("Saisissez votre proposition , %d caractères :\n",messages_length);
	char saisie[messages_length];
	scanf("%s",saisie);
	printf("Vous avez rentré : %s\n",saisie);
	h_writes(num_socket,saisie,messages_length);

	char longueur_nombre_bp[1];
	int lgru = h_reads(num_socket,longueur_nombre_bp,1);
	printf("%d\n",lgru);
	printf("lg_nb_bp = %c\n",longueur_nombre_bp[0]);//PK longueur_nombre_bp = 110 ??????????
	int lg_nb_bp=longueur_nombre_bp[0]-'0';

	h_writes(num_socket," ",1);

	char nombre_bien_placee[lg_nb_bp];
	h_reads(num_socket,nombre_bien_placee,lg_nb_bp);

	h_writes(num_socket,"a",1);

	char longueur_nombre_mp[1];
	h_reads(num_socket,longueur_nombre_mp,1);
	int lg_nb_mp=longueur_nombre_mp[0]-'0';

	h_writes(num_socket," ",1);

	char nombre_mal_placee[lg_nb_mp];
	h_reads(num_socket,nombre_mal_placee,lg_nb_mp);

	printf("Nombre de couleurs bien placées :");
	for(int i=0;i<lg_nb_bp;i++){
		printf("%c",nombre_bien_placee[i]);
	}
	printf("\n");
	printf("Nombre de couleurs mal placées :");
	for(int i=0;i<lg_nb_mp;i++){
		printf("%c",nombre_mal_placee[i]);
	}
	printf("\n");
	char keep[1];
	h_reads(num_socket,keep,1);
	printf("keep : %c\n",keep[0]);
	if (keep[0]=='1'){
		printf("Vous avez gagné ! Bravo !\n");
		gagne=1;
	}
	else{
		printf("Vous n'avez pas encore gagné ! Continuez !\n");
	}
}


/*printf("Longueur message : ");
int lg_message;
scanf("%d",&lg_message);
char message[lg_message];
printf("Rentrer le message :");
scanf("%s",message);
printf("\n");
for(int i=0;i<lg_message;i++){
	printf("%c",message[i]);
}*/
/*char buffer_recpt[level];
char quit;
while(1){
	printf(" ok ");
	h_reads(num_socket,buffer_recpt,level);
	printf("Reçu : %s\n",buffer_recpt);
	printf("Voulez quitter ? (y) pour quitter (n) pour continuer\n");
	scanf("%c\n",&quit);
	if(quit=='y'){
		break;
	}
}*/
/*printf("Voulez vous envoyer un message ?\n");
int tmp;
scanf("%d",&tmp);
int lg_message = 20;
char message[20] = "Salut comment tu vas";
printf("\n");
printf("\n");
h_writes (num_socket,message, lg_message) ;*/
}

/*****************************************************************************/
