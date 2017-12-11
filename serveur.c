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

void serveur_appli(char *service) {

	/*Cette partie sert à initialiser la socket serveur qui va recevoir les demandes de connexion*/

	int socket_serv = socket(AF_INET, SOCK_STREAM, 0); //création de la socket
	struct sockaddr_in ssin = {0}; /* initialise la structure avec des 0 */
	const char *hostname = service;
	ssin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */
	ssin.sin_family = AF_INET;
	ssin.sin_port = htons(PORT);
	bind(socket_serv, (struct sockaddr *) &ssin, sizeof ssin);
	printf("Serveur : Bind fait\n");
	listen(socket_serv, 5);
	printf("Serveur : Listen fait\n");
	struct sockaddr_in csin = {0};


	int clients[5]; //Va stocker les id des sockets

	int nb_clients = 0;

	int sock_max = socket_serv;

    char buffer[1024];

	fd_set readfs;

	while (1) {
		FD_ZERO(&readfs);
		FD_SET(STDIN_FILENO, &readfs);
		FD_SET(socket_serv, &readfs);

		for(int i=0;i<nb_clients;i++){
			FD_SET(clients[i],&readfs);
		}

		select(socket_serv+1, &readfs, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &readfs))
		{
			//lecture_clavier();
		}

		else if(FD_ISSET(socket_serv,&readfs)){
            printf("cond1\n");
			int nv_client;
			int sinsize = sizeof csin;

			nv_client = accept(socket_serv, (struct sockaddr *) &csin, &sinsize);

			printf("Nouvelle connexion détéctée\n");

            //int n = recv(nv_client, buffer, sizeof buffer - 1, 0);

            printf("Reçu\n");

            /*if(n == -1){
				continue;
			}*/

			FD_SET(nv_client,&readfs);

			if(nv_client>sock_max){
				sock_max = nv_client;
			}

            printf("id socket : %d\n",nv_client);

		}

        else{
            for(int i=0;i<5;i++){
                if(FD_ISSET(clients[i],&readfs)){
                    printf("Message reçu, socket %d\n",clients[i]);
                    int n = recv(clients[i], buffer, sizeof buffer - 1, 0);
                    buffer[n] = '\0';
                    if(n == 0){
                        close(clients[i]);
                        memmove(clients + i, clients + i + 1, (nb_clients - i - 1) * sizeof(int));
                        nb_clients--;
                    }
                    else{

                        for(int j = 0; j < nb_clients; j++){
                            if(clients[i] != clients[j])
                            {
                                write(clients[i], buffer, n);
                            }
                        }
                    }
                    break;
                }
            }
        }
	}

    for(int i=0;i<nb_clients;i++){
        close(clients[i]);
    }
    close(socket_serv);
}
