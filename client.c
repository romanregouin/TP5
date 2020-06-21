/******************************************************************************/
/*			Application: pendu */
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : REGOUIN Roman ANDRIEUX Liam
 */
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

/* Structure qui contient l'etat actuel du mot à trouver */
typedef struct {
  char* mot;  // Tableau qui contient le mot a trouver (il sera initialisé avec
              // des '0')
  char* lettre;  // tableau contenant la liste de toutes les lettres données par
                 // le joueur
  char* lettre_bonne;  // tableau contenant la liste des lettres bonnes trouvés
  int try
    ;       // le nombre de lettre donné
  int bon;  // le nombre de lettre bonne trouvé
  int nb;   // la taille du mot a trouvé
} Actuel;

/* Initialise la strucutre */
// taille: Sert a initialisé la taille du mot a trouvé
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

/* Actualise les essaies et les lettres qui ont été utilises */
Actuel ajouter_actuel(Actuel a, char* c) {
  a.lettre[a.try] = c[0];
  a.try
    ++;
  return a;
}

/* Prend une lettre reponse et actualise l'avancement du mot cherché via la
structure a et met a jour le tableau indices avec les indices des occurences de
la lettre dans le mot cherché et renvoie ce nombre d'occurence */
int actualiser(char* reponse, int indice, Actuel* a) {
  if (indice == -1) return -1;
  if (string_index_of(a->lettre_bonne, 0, reponse[0]) == -1) {
    a->lettre_bonne[a->bon] = reponse[0];
    a->bon++;
  }
  a->mot[indice] = reponse[0];
  return 1;
}

/* affiche le mot dans l'etat actuel et renvoie 1 si le mot n'est pas encore
 * trouve 0 sinon */
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

/* Affiche l'etat actuel du pendu */
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

/* lit ce que l'utilisateur a ecrit dans le terminal */
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

/* Affiche les lettres deja utilisees */
void affichage_lettre(Actuel a) {
  int i;
  printf("\n\nVoici la(es) lettre(s) déjà utilisée(s):");
  for (i = 0; i < a.try - 1; i++) {
    printf(" %c -", a.lettre[i]);
  }
  printf(" %c \n\n", a.lettre[i]);
}

void espace() {
  printf(
      "\n______________________________________________________________________"
      "__________________\n\n\n");
}

void client_appli(char* serveur, char* service) {
  // Initialisation des sockets
  int id_socket = h_socket(AF_INET, SOCK_DGRAM);
  struct sockaddr_in *p_adr_serv, *p_adr_distant;
  char myname[17];
  gethostname(myname, 16);  // Recupere l'IP de la machine local
  adr_socket(service, myname, SOCK_DGRAM, &p_adr_distant);
  adr_socket(service, SERVEUR_DEFAUT, SOCK_DGRAM, &p_adr_serv);
  h_bind(id_socket, p_adr_serv);

  // Envoi d'une demande d'initialisation du jeu (INIT) au serveur
  char tampon[20];
  tampon[0] = 'I';
  tampon[1] = 'N';
  tampon[2] = 'I';
  tampon[3] = 'T';
  int nb = h_sendto(id_socket, tampon, 4, p_adr_distant);

  // Reception et traitement du message du server qui contient le nombre de
  // lettre touvee et leur position pour la lettre teste
  nb = h_recvfrom(id_socket, tampon, 5, NULL);

  // On recupère dans le tampon la taille du mot mystere
  int taille = 0;
  for (int i = 0; i < nb; i++) taille = (int)tampon[i];

  // Initialisation des variables utilisé pour l'application client et la
  // récupération des lettres dans le terminal
  buffer = malloc(100);
  setbuf(stdout, NULL);
  int continuer = 1;
  char reponse1[1];
  char* reponse2 = malloc(3);
  int try
    , gagner, indice, k, trouver;
  try
    = 0;
  printf(
      " 	Bonjour est bienvenue dans le jeu du Pendue !\n 	Vous "
      "avez 10 coups pour deviner le mot !\n 	Bonne chance !\n");
  gagner = 1;
  while (continuer) {
    // On initialise la structure qui permet de savoir où on en est dans le jeu
    Actuel a = init_actuel(taille);
    // On affiche le mot avec aucune lettre encore trouvé
    gagner = affichage(a);
    // On récupere ce que le joueur écrit dans le terminal
    char* tmp = lire_ligne();
    // On ne va utiliser que la première lettre donné par le joueur
    reponse1[0] = tmp[0];
    free(tmp);
    // On met a jour notre structure pour se souvenir des lettres dejà utilisé
    a = ajouter_actuel(a, reponse1);
    while (gagner && try <= 10) {
      printf("\n");
      // On envoie la réponse du joueur au serveur
      nb = h_sendto(id_socket, reponse1, 1, p_adr_distant);
      // On recupere le Traitement du Serveur au format (<Nombre de lettre
      // présent dans le mot> - <indice de la 1ere lettre> - ... - <indice de la
      // nieme lettre> -)
      nb = h_recvfrom(id_socket, tampon, 20, NULL);
      k = 0;
      trouver = 0;
      // On va donc traiter le format de réponse pour mettre a jour la structure
      // du jeu Ce premier while permet de récupérer le nombre de lettre
      // présente dans le mot
      while (tampon[k] != '-') {
        trouver = trouver * 10 + (int)tampon[k];
        k++;
      }
      k++;
      // Si aucune lettre présente on arrete
      if (k == 0) trouver = 0;
      // Sinon on continue de parcourir le message en récupérant les indices
      for (int l = 0; l < trouver; l++) {
        indice = 0;
        while (tampon[k] != '-') {
          indice = indice * 10 + (int)tampon[k];
          k++;
        }
        // C'est ici qu'on met a jour le mot avec les lettres connues
        actualiser(reponse1, indice, &a);
        k++;
      }
      // Si la lettre est pas présente on incrémente le nombre de rater et on
      // affiche l'etat actuel du mot mystere
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

      // Si le joueur n'a pas encore perdu ou gagner on lui demande une nouvelle
      // lettre
      if (gagner == 1 && try < 10) {
        do {
          affichage_lettre(a);
          printf("?");
          char* tmp2 = lire_ligne();
          reponse1[0] = tmp2[0];
          free(tmp);
          if (string_index_of(a.lettre, 0, reponse1[0]) != -1) {
            printf("        La lettre a déjà été donné !\n");
          }
        } while (string_index_of(a.lettre, 0, reponse1[0]) != -1);
      }
      a = ajouter_actuel(a, reponse1);
    }
    // Ici on a quitter le while donc on a soit gagner ou perdu la partie
    if (try <= 10) {
      printf(" 	Bravo, vous avez gagné !!!! \n");
    } else {
      printf(
          "	 Dommage vous avez dépassé les 10 essaies ! C'est perdu pour "
          "cette fois !\n ");
    }
    continuer = 0;
  }
  // Maintenant que la partie est terminé on prévient le serveur de la fin de
  // partie et on ferme la socket
  tampon[0] = 'E';
  tampon[1] = 'N';
  tampon[2] = 'D';
  nb = h_sendto(id_socket, tampon, 3, p_adr_distant);
  h_close(id_socket);
  free(buffer);
  free(reponse2);
  printf("Au revoir !!!\n");
}