#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "string.h"

#define STDIN 1
#define Max 10
#define Try 10

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

int actualiser(char* reponse, char* deviner, Actuel* a) {
  int j;
  for (j = 0; j < string_length(deviner); j++) {
    if (reponse[0] == deviner[j]) {
      if (string_index_of(a->lettre_bonne, 0, reponse[0]) == -1) {
        a->lettre_bonne[a->bon] = reponse[0];
        a->bon++;
      }
      return 1;
    }
  }
  return -1;
}

int affichage(Actuel a, char* deviner) {
  int i, j, n, h;
  h = 0;
  for (i = 0; i < string_length(deviner); i++) {
    n = 0;
    for (j = 0; j < a.bon; j++) {
      if (deviner[i] == a.lettre_bonne[j]) {
        printf("%c", a.lettre_bonne[j]);
        n++;
      }
    }
    if (n == 0) {
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

int main(int nargs, char** args) {
  buffer = malloc(100);
  setbuf(stdout, NULL);
  int continuer = 1;
  char reponse1[1];
  char* reponse2 = malloc(3);
  int random, try
    , gagner;
  char* deviner = malloc(Max);
  // initlalize the random generator
  srand(clock());
  char* d[10] = {"savon",    "papillon", "xilophone", "tulukmatou", "nathan",
                 "rigolade", "saumon",   "pecule",    "yo",         "kayak"};
  try
    = 0;
  printf(
      " 	Bonjour est bienvenue dans le jeu du Pendue !\n 	Vous "
      "avez 10 coups pour deviner le mot !\n 	Bonne chance !\n");
  random = rand() % 10;
  string_copy(deviner, d[random]);
  gagner = 1;
  while (continuer) {
    Actuel a = init_actuel();
    gagner = affichage(a, deviner);
    printf("?");
    reponse1[0] = lire_ligne()[0];
    a = ajouter_actuel(a, reponse1);
    while (gagner && try <= 10) {
      printf("\n");
      if (actualiser(reponse1, deviner, &a) == 1) {
        gagner = affichage(a, deviner);
        printf("	Vous avez trouvez une lettre !\n");
      } else {
        try
          ++;
        gagner = affichage(a, deviner);
        affichage_pendue(try);
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
        random = rand() % 10;
        string_copy(deviner, d[random]);
        gagner = 1;
        try
          = 0;
        free(a.lettre);
        free(a.lettre_bonne);
        printf("	C'est repartie pour un tour !!\n");
      } else {
        printf(
            "        Il faut taper 'oui' pour relancer ou taper 'non' pour "
            "arreter.\n");
      }
    } while ((strcmp(reponse2, "oui") != 0) && (strcmp(reponse2, "non") != 0));
  }
  printf("Au revoir !!!\n");
  return 0;
}
