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

/* une couleur est represente par un char tel que :
R = rouge
Y = jaune
G = vert
B = bleu
O = orange
W = blanc
V = violet
*/

char convert_int_coul(int num){ // on a besoin d'une fonction qui pour un entier entre 0 et 6 renvoie une couleur
switch(num){
	case 0 :
	return('R');
	case 1 :
	return('Y');
	case 2 :
	return('G');
	case 3 :
	return('B');
	case 4 :
	return('O');
	case 5 :
	return('W');
	default :
	return('V');
}
}

// permet de generer un entier aleatoire entre 0 et 6 compris
int rand_a_b(int a, int b){
	return rand()%(b-a) +a;
}

// on genere un entier aleatoire et on le convertit en une couleur grace aux fonctions du desssus
void *cle_aleatoire(int n, char *cle){
	for (int i = 0; i < n; ++i){
		cle[i]= convert_int_coul(rand_a_b(0, 7));
	}
}

/* permet de comparer les char des deux mots, on parcourt le mot envoyé par le client, si la couleur a l indice i est la meme pour le client
et le serveur on incremente bien_place, sinon on regarde si la couleur de l indice i du client est presente ailleurs dans le mot du serveur
avec une boucle for et on incremente mal_place si la couleur est presente ailleurs*/
void compare(char *cle, char *client, int n, int *bien_place, int *mal_place){
	/*printf("-----------------------\n");
	printf("Je début la comparaison\n");
	printf("-----------------------\n");*/
	char lettre;
	for (int i = 0; i < n; ++i){
		lettre = client[i];;
		if(client[i] == cle[i]){
			//printf("J'ai trouvé une lettre bien placée\n");
			(*bien_place)++;
		}
		else{
			for (int j = 0; j < n; ++j){
				if(cle[j]==lettre){
					//printf("J'ai trouvé une lettre mal placée\n");
					(*mal_place)++;
					break;
				}
			}
		}
	}
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

	printf("Le client veut deviner %d couleurs\n", n);

	char cle[n];
	cle_aleatoire(n, cle);
	printf("On a crée la chaine de couleur ci dessous\n");
	for (int i = 0; i < n; ++i)
	{
		printf("%c", cle[i]);
	}
	printf("\n");
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
		compare(cle, client, n, &bien_place, &mal_place);
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
		/* on envoie le nombre de bien place et mal place, la gestion de la victoire est faite chez le client et le serveur
		en comparant n et bien place */
		h_writes(num_socket_client, lgtmp1, 1);
		taille_effective = h_reads(num_socket_client, attente, 1);
		h_writes (num_socket_client, tampon1, lg_tmp1) ; // on envoie
		taille_effective = h_reads(num_socket_client, attente, 1); // ne pas ecraser avec le 2eme write la valeur envoyé par le 1er
		h_writes(num_socket_client, lgtmp2, 1);
		taille_effective = h_reads(num_socket_client, attente, 1);
		h_writes (num_socket_client, tampon2, lg_tmp2) ;
		if(bien_place==n){
			printf("Le client a gagne au bout de %d coup !\n", nbr_coup);
			gagne=1;
		}
		else{
			printf("Le client a faux \n");
			nbr_coup++;
		}
		char trans[1];
		sprintf(trans, "%d", gagne);
		h_writes(num_socket_client,trans,1);
		h_close(num_socket_client);
		h_close(num_socket);
		//gagne = 1;
	}

}

/* fin des echanges avec le client */

/******************************************************************************/
