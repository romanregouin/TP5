/******************************************************************************/
/*			Application: pendu			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  REGOUIN Roman ANDRIEUX Liam						      */
/*		Date :  21/06/2020						      */
/*									      */
/******************************************************************************/

//#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <time.h>
#include "fon.h" /* Primitives de la boite a outils */
#include "string.h"

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"
#define Max 10
#define Try 10
#define STDIN 1
#define NB_MOTS 10

void serveur_appli(char* service); /* programme serveur */

/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc, char* argv[]) {
  char* service = SERVICE_DEFAUT; /* numero de service par defaut */

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

char* buffer = NULL;

int read_char(char* ch) { return read(STDIN, ch, 1); }

/* Structure qui contient l'etat actuel du mot à trouver */
typedef struct {
  char* lettre;
  char* lettre_bonne;
  int try;
  int bon;
} Actuel;

/* Initialise la strucutre */
Actuel init_actuel() {
  Actuel a;
  a.lettre = malloc(sizeof(char) * Max);
  a.lettre_bonne = malloc(sizeof(char) * Max);
  a.try = 0;
  a.bon = 0;
  return a;
}

/* Actualise les essaies et les lettres qui ont été utilises */
Actuel ajouter_actuel(Actuel a, char* c) {
  a.lettre[a.try] = c[0];
  a.try ++;
  return a;
}

/* Prend une lettre reponse et actualise l'avancement du mot cherché via la structure a 
et met a jour le tableau indices avec les indices des occurences de la lettre dans le mot 
cherché et renvoie ce nombre d'occurence */
int actualiser(int* indices, char* reponse, char* deviner, Actuel* a) {
  int j;
  int nbIndices = 0;
  for (j = 0; j < string_length(deviner); j++) {
    if (reponse[0] == deviner[j]) {
      if (string_index_of(a->lettre_bonne, 0, reponse[0]) == -1) {
        a->lettre_bonne[a->bon] = reponse[0];
        a->bon++;
      }
      indices[nbIndices] = j;
      nbIndices++;
    }
  }
  return nbIndices;
}

/* Initialise le jeu : tire aleatoirement le mot a devenier parmis sa banque de donnee et le renvoi*/
char* initGame() {
  char* deviner = malloc(Max);
  srand(clock());
  char* d[NB_MOTS] = {"savon",  "papillon", "xilophone", "tulukmatou",
                      "nathan", "rigolade", "saumon",    "pecule",
                      "yo",     "kayak"};
  int random = rand() % NB_MOTS;
  string_copy(deviner, d[random]);
  return deviner;
}

/******************************************************************************/
void serveur_appli(char* service)

/* Procedure correspondant au traitement du serveur de votre application */

{
  // Initialisation des sockets
  struct sockaddr_in *p_adr_serv,
      *p_adr_distant = malloc(sizeof(struct sockaddr_in));
  int started = 0;
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  adr_socket(service, NULL, SOCK_DGRAM, &p_adr_serv);
  h_bind(id_socket, p_adr_serv);

  // Initialisation des Variables
  char bufferReception[4];
  char bufferEmission[20];
  char* word;
  Actuel a;
  int* indices = malloc(10 * sizeof(int));
  int nbIndices = 0;

  // Premiere reception d'un message du client
  int nb = h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);

  // Boucle tant que le message recu n'est pas une demande d'initialisation du jeu (INIT)
  while (!started) {
    // Si le message est INIT alors on initialise le jeu et en envoi au client
    // le nombre de lettre du mot a trouver
    if (myStringCmp(bufferReception, "INIT")) {
      printf("Init client reçu\n");
      word = initGame();
      a = init_actuel();
      bufferEmission[0] = (char)string_length(word);
      printf("Le mot a trouver est :  ");
      for (int k = 0; k < string_length(word); k++) {
        printf("%c", word[k]);
      }
      printf("\n");
      h_sendto(id_socket, bufferEmission, 1, p_adr_distant);
      started++;
    } else { // Sinon on se remet en attente d'une demande d'initialisation de jeu (INIT)
      h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);
    }
  }

  // On répéte la procedure : reception d'une lettre, traitement de celle-ci, envoie au client 
  // le nombre d'occurence de la lettre si elle est dans le mot a trouver et leurs position dans 
  // le mot, cette boucle se termine quand le message recu est END
  while (1) {
    nb = h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);
    printf("Message reçu : ");
    for (int i = 0; i < nb; i++) {
      printf("%c", bufferReception[i]);
    }
    printf("\n");
    if (myStringCmp(bufferReception, "END")) {
      h_close(id_socket);
      printf("\n");
      break;
    }
    a = ajouter_actuel(a, bufferReception);
    nbIndices = actualiser(indices, bufferReception, word, &a);
    int writePosition = 0;
    if (nbIndices == 0) {
      bufferEmission[writePosition] = 0;
      writePosition++;
      bufferEmission[writePosition] = '-';
    } else {
      bufferEmission[writePosition] = (char)nbIndices;
      writePosition++;
      for (int i = 0; i < nbIndices; i++) {
        bufferEmission[writePosition] = '-';
        writePosition++;
        bufferEmission[writePosition] = (char)indices[i];
        writePosition++;
      }
      bufferEmission[writePosition] = '-';
    }
    h_sendto(id_socket, bufferEmission, writePosition + 1, p_adr_distant);
  }
  printf("Fin du jeu\n");
  free(p_adr_distant);
  free(indices);
}
