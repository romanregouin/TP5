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

//#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h" /* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

void serveur_appli(char *service); /* programme serveur */

/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc, char *argv[]) {
  char *service = SERVICE_DEFAUT; /* numero de service par defaut */

  /* Permet de passer un nombre de parametre variable a l'executable */
  switch (argc) {
    case 1:
      printf("defaut service = %s\n", service);
      break;
    case 2:
      service = argv[1];
      break;

    default:
      printf("Usage:serveur service (nom ou port) \n");
      exit(1);
  }

  /* service est le service (ou numero de port) auquel sera affecte
  ce serveur*/

  serveur_appli(service);
}

/******************************************************************************/
void serveur_appli(char *service)

/* Procedure correspondant au traitement du serveur de votre application */

{
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  struct sockaddr_in *p_adr_serv, *p_adr_distant;
  adr_socket(service, NULL, SOCK_DGRAM, &p_adr_serv);
  adr_socket(service, SERVEUR_DEFAUT, SOCK_DGRAM, &p_adr_distant);
  h_bind(id_socket, p_adr_serv);
  char *tampon = "salut";
  int nb = h_sendto(id_socket, tampon, 5, p_adr_distant);
  printf("%d\n", nb);
}

/******************************************************************************/
