/******************************************************************************/
/*			Application: ... */
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/

//#include <curses.h> /* Primitives de gestion d'ecran */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "fon.h" /* primitives de la boite a outils */
#include "string.h"

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"
#define Max 10
#define Try 10
#define STDIN 1

void client_appli(char* serveur, char* service);

/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char* argv[]) {
  char* serveur = SERVEUR_DEFAUT; /* serveur par defaut */
  char* service =
      SERVICE_DEFAUT; /* numero de service par defaut (no de port) */

  /* Permet de passer un nombre de parametre variable a l'executable */
  switch (argc) {
    case 1: /* arguments par defaut */
      printf("serveur par defaut: %s\n", serveur);
      printf("service par defaut: %s\n", service);
      break;
    case 2: /* serveur renseigne  */
      serveur = argv[1];
      printf("service par defaut: %s\n", service);
      break;
    case 3: /* serveur, service renseignes */
      serveur = argv[1];
      service = argv[2];
      break;
    default:
      printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
      exit(1);
  }

  /* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
  /* service le numero de port sur le serveur correspondant au  */
  /* service desire par le client */

  client_appli(serveur, service);
}

char* buffer = NULL;

int read_char(char* ch) { return read(STDIN, ch, 1); }

typedef struct {
  char* mot;
  char* lettre;
  char* lettre_bonne;
  int try
    ;
  int bon;
  int nb;
} Actuel;

Actuel init_actuel(int taille) {
  Actuel a;
  a.mot = malloc(sizeof(char) * taille);
  for (int i = 0; i < taille; i++) a.mot[i] = '0';
  a.lettre = malloc(sizeof(char) * Max);
  a.lettre_bonne = malloc(sizeof(char) * Max);
  a.try
    = 0;
  a.bon = 0;
  a.nb = taille;
  return a;
}

Actuel ajouter_actuel(Actuel a, char* c) {
  a.lettre[a.try] = c[0];
  a.try
    ++;
  return a;
}

int actualiser(char* reponse, int indice, Actuel* a) {
  if (indice == -1) return -1;
  if (string_index_of(a->lettre_bonne, 0, reponse[0]) == -1) {
    a->lettre_bonne[a->bon] = reponse[0];
    a->bon++;
  }
  a->mot[indice] = reponse[0];
  return 1;
}

int affichage(Actuel a) {
  int i, h;
  h = 0;
  for (i = 0; i < a.nb; i++) {
    if (a.mot[i] != '0') {
      printf("%c", a.mot[i]);
    } else {
      printf("-");
      h++;
    }
  }
  printf("\n");
  if (h != 0) return 1;
  return 0;
}

void affichage_pendue(int try) {
  printf("\n");
  if (try == 1) {
    printf("                \n");
    printf("                \n");
    printf("                \n");
    printf("                \n");
    printf("	      ____\n");
  } else if (try == 2) {
    printf("                |\n");
    printf("                |\n");
    printf("                |\n");
    printf("                |\n");
    printf("	      __|__\n");
  } else if (try == 3) {
    printf("                 _____\n");
    printf("                |\n");
    printf("                |\n");
    printf("                |\n");
    printf("              __|__\n");
  } else if (try == 4) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |\n");
    printf("                |\n");
    printf("              __|__\n");
  } else if (try == 5) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |     O\n");
    printf("                |\n");
    printf("              __|__\n");
  } else if (try == 6) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |    \\O \n");
    printf("                |\n");
    printf("              __|__\n");
  } else if (try == 7) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |    \\O/\n");
    printf("                |\n");
    printf("              __|__\n");
  } else if (try == 8) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |    \\O/\n");
    printf("                |     |\n");
    printf("              __|__\n");
  } else if (try == 9) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |    \\O/\n");
    printf("                |     |\n");
    printf("              __|__  /\n");
  } else if (try == 10) {
    printf("                 _____\n");
    printf("                |     |\n");
    printf("                |    \\O/\n");
    printf("                |     |\n");
    printf("              __|__  / \\ \n");
  }
  printf("\nLa lettre n'est pas dans le mot, dommage !\n");
}

char* lire_ligne() {
  int len = 0;
  char* str;
  char ch;
  do {
    if (len >= 100) break;
    if (read_char(&ch) != 1) {
      if (len == 0) return " ";
      break;
    }
    buffer[len] = ch;
    len = len + 1;
  } while (ch != '\n');

  len = len - 1;
  buffer[len] = '\0';

  str = malloc(len + 1);
  for (int i = 0; i <= len; i++) str[i] = buffer[i];
  if (len == 0 && str[0] == '\0') str[0] = ' ';
  return str;
}

void affichage_lettre(Actuel a) {
  int i;
  printf("\n\nVoici la(es) lettre(s) déjà utilisée(s):");
  for (i = 0; i < a.try - 1; i++) {
    printf(" %c -", a.lettre[i]);
  }
  printf(" %c \n\n", a.lettre[i]);
}

void read_3_char(char* ch) {
  char* str = malloc(100);
  strcpy(str, lire_ligne());
  ch[0] = str[0];
  ch[1] = str[1];
  ch[2] = str[2];
  ch[3] = '\0';
}

void espace() {
  printf(
      "\n______________________________________________________________________"
      "__________________\n\n\n");
}

void client_appli(char* serveur, char* service) {
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  struct sockaddr_in *p_adr_serv, *p_adr_distant;
  adr_socket(service, NULL, SOCK_DGRAM, &p_adr_serv);
  adr_socket(service, SERVEUR_DEFAUT, SOCK_DGRAM, &p_adr_distant);
  h_bind(id_socket, p_adr_serv);
  char tampon[20];
  tampon[0] = 'I';
  tampon[1] = 'N';
  tampon[2] = 'I';
  tampon[3] = 'T';
  int nb = h_sendto(id_socket, tampon, 4, p_adr_distant);
  nb = h_recvfrom(id_socket, tampon, 5, p_adr_distant);
  int taille = 0;
  for (int i = 0; i < nb; i++) taille = taille * 10 + tampon[i] - '0';

  buffer = malloc(100);
  setbuf(stdout, NULL);
  int continuer = 1;
  char reponse1[1];
  char* reponse2 = malloc(3);
  int try
    , gagner, indice, k, trouver;
  // initlalize the random generator

  try
    = 0;
  printf(
      " 	Bonjour est bienvenue dans le jeu du Pendue !\n 	Vous "
      "avez 10 coups pour deviner le mot !\n 	Bonne chance !\n");
  gagner = 1;
  while (continuer) {
    Actuel a = init_actuel(taille);
    gagner = affichage(a);
    printf("?");
    reponse1[0] = lire_ligne()[0];
    a = ajouter_actuel(a, reponse1);
    while (gagner && try <= 10) {
      printf("\n");

      nb = h_sendto(id_socket, reponse1, 1, p_adr_distant);
      nb = h_recvfrom(id_socket, tampon, 20, p_adr_distant);
      k = 0;
      trouver = 0;
      while (tampon[k] == '-') {
        trouver = trouver * 10 + tampon[k] - '0';
        k++;
      }

      for (int l = 0; l < trouver; l++) {
        while (tampon[k] == '-') {
          indice = indice * 10 + tampon[k] - '0';
        }
        actualiser(reponse1, indice, &a);
      }
      if (trouver == 0) {
        try
          ++;
        gagner = affichage(a);
        affichage_pendue(try);
      } else {
        gagner = affichage(a);
        printf("	Vous avez trouvez une lettre !\n");
      }

      espace();
      if (gagner == 1 && try < 10) {
        do {
          affichage_lettre(a);
          printf("?");
          reponse1[0] = lire_ligne()[0];
          if (string_index_of(a.lettre, 0, reponse1[0]) != -1) {
            printf("        La lettre a déjà été donné !\n");
          }
        } while (string_index_of(a.lettre, 0, reponse1[0]) != -1);
      }
      a = ajouter_actuel(a, reponse1);
    }
    if (try <= 10) {
      printf(
          " 	Bravo, vous avez gagné !!!!  \n 	Pour relancer une "
          "partie taper 'oui' sinon taper 'non'\n");
    } else {
      printf(
          "	 Dommage vous avez dépassé les 10 essaies ! C'est perdu pour "
          "cette fois !\n         Pour relancer une partie taper 'oui' sinon "
          "taper 'non'\n");
    }
    do {
      read_3_char(reponse2);
      if (strcmp(reponse2, "non") == 0) {
        continuer = 0;
      } else if (strcmp(reponse2, "oui") == 0) {
        tampon[0] = 'I';
        tampon[1] = 'N';
        tampon[2] = 'I';
        tampon[3] = 'T';
        int nb = h_sendto(id_socket, tampon, 4, p_adr_distant);
        nb = h_recvfrom(id_socket, tampon, 5, p_adr_distant);
        int taille = 0;
        for (int i = 0; i < nb; i++) taille = taille * 10 + tampon[i] - '0';
        gagner = 1;
        try
          = 0;
        free(a.lettre);
        free(a.lettre_bonne);
        free(a.mot);
        printf("	C'est repartie pour un tour !!\n");
      } else {
        printf(
            "        Il faut taper 'oui' pour relancer ou taper 'non' pour "
            "arreter.\n");
      }
    } while ((strcmp(reponse2, "oui") != 0) && (strcmp(reponse2, "non") != 0));
  }
  printf("Au revoir !!!\n");
}

/*****************************************************************************/
/* procedure correspondant au traitement du client de votre application */

/*
void client_appli(char* serveur, char* service)



{
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  struct sockaddr_in *p_adr_serv, *p_adr_distant;
  adr_socket(service, NULL, SOCK_DGRAM, &p_adr_serv);
  adr_socket(service, SERVEUR_DEFAUT, SOCK_DGRAM, &p_adr_distant);
  h_bind(id_socket, p_adr_serv);
  char* tampon = "salut";
  int nb = h_sendto(id_socket, tampon, 5, p_adr_distant);
  printf("%d\n", nb);
}
*/
//    /*****************************************************************************/
