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
#include "string.h"

#include "fon.h" /* Primitives de la boite a outils */

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

typedef struct {
  char* lettre;
  char* lettre_bonne;
  int try
    ;
  int bon;
} Actuel;

Actuel init_actuel() {
  Actuel a;
  a.lettre = malloc(sizeof(char) * Max);
  a.lettre_bonne = malloc(sizeof(char) * Max);
  a.try
    = 0;
  a.bon = 0;
  return a;
}

Actuel ajouter_actuel(Actuel a, char* c) {
  a.lettre[a.try] = c[0];
  a.try
    ++;
  return a;
}

int actualiser(int* indices, char* reponse, char* deviner, Actuel* a) {
  int j;
  int nbIndices = 0;
  for (j = 0; j < string_length(deviner); j++) {
    if (reponse[0] == deviner[j]) {
      if (string_index_of(a->lettre_bonne, 0, reponse[0]) == -1) {
        a->lettre_bonne[a->bon] = reponse[0];
        a->bon++;
        indices[nbIndices] = j;
        nbIndices++;
      }
      return nbIndices;
    }
  }
  return -1;
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

int read_3_char(char* ch) {
  char* str = malloc(100);
  strcpy(str, lire_ligne());
  ch[0] = str[0];
  ch[1] = str[1];
  ch[2] = str[2];
  ch[3] = '\0';
}

char* initGame(){
  char* deviner = malloc(Max);
  srand(clock());
  char* d[NB_MOTS] = {"savon",    "papillon", "xilophone", "tulukmatou", "nathan",
                 "rigolade", "saumon",   "pecule",    "yo",         "kayak"};
  int try = 0;
  int random = rand() % NB_MOTS;
  string_copy(deviner, d[random]);
  return deviner;
}

/******************************************************************************/
void serveur_appli(char* service)

/* Procedure correspondant au traitement du serveur de votre application */

{
  struct sockaddr_in *p_adr_serv, *p_adr_distant;
  int started = 0;
  int ended = 0;
  int try = 0;
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  adr_socket(service, NULL, SOCK_DGRAM, &p_adr_distant);
  adr_socket(service, SERVEUR_DEFAUT, SOCK_DGRAM, &p_adr_serv);
  h_bind(id_socket, p_adr_serv);
  char bufferReception[4];
  char bufferEmission[20];
  char* word;
  Actuel a;
  int* indices = malloc(10*sizeof(int));
  int nb = h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);
  int nbIndices = 0;
  while(!started){
    if(myStringCmp(bufferReception,"INIT")){
      word = initGame();
      a = init_actuel();
      started++;
    }else{
      nb = h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);
    }
  }
  while(1){
    nb = h_recvfrom(id_socket, bufferReception, 4, p_adr_distant);
    if(myStringCmp(bufferReception, "END")){
      h_close(id_socket);
      return;
    }
    a = ajouter_actuel(a, bufferReception[0]);
    nbIndices = actualiser(indices,bufferReception[0], word, &a);
    if(nbIndices==-1){
      bufferEmission[0] = 0;
      bufferEmission[1] = 1;
    }else{
      bufferEmission[0] = nbIndices;
      int writePosition = 1;
      for(int i=0;i<nbIndices;i++){
        bufferEmission[writePosition] = '-';
        writePosition++;
        bufferEmission[writePosition] = indices[i];
        writePosition++;
      }
      bufferEmission[writePosition] = '-';
    }
  }
}

/******************************************************************************/
