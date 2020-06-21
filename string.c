#include "string.h"

/**
 * @return la longueur de la chaine de caractères, c'est à dire le nombre
 * de caractères qu'elle contient.
 * Exemple: "" -> length = 0
 *          "abc" -> length = 3
 */
int string_length(char* s) {
  int i = 0;
  while (s[i] != '\0') i++;
  return i;
}

/**
 * Copie la second chaîne sur la première.
 * @param s1 est la première chaîne
 * @param s2 est la seconde chaîne
 */
void string_copy(char* s1, char* s2) {
  int i, l2;
  l2 = string_length(s2);
  i = 0;
  while (i < l2) {
    s1[i] = s2[i];
    i++;
  }
  s1[i] = '\0';
}

/**
 * Concatène les deux chaînes de caractères, ajoutant la seconde à la fin
 * de la première.
 * @param s1 est la première chaîne
 * @param s2 est la seconde chaîne
 */
/*void string_concat(char* s1, char* s2) {
        int l1,l2,i;
        l1=string_length(s1);
        l2=string_length(s2);
        char* s3 = malloc(sizeof(s1)*l1+sizeof(s2)*l2);
        i=0;
        while(i<l1||i<l2){
                if(i<l1)s3[i]=s1[i];
                if(i<l2)s3[l1+i]=s2[i];
                i++;
        }
        string_copy(s1,s3);
}*/

/**
 * Compare les deux chaînes de caractères
 * @param s1
 * @param s2
 * @return 0 si les deux chaînes sont identiques.
 *         -1 si la chaîne s1 est lexicographiquement inférieur à la chaîne s2
 *         1 si  la chaîne s1 est lexicographiquement supérieur à la chaîne s2
 */
int string_cmp(char* s1, char* s2) {
  int i, l1, l2;
  l1 = string_length(s1);
  l2 = string_length(s2);
  if (l1 > l2) return 1;
  if (l1 < l2) return -1;
  i = 0;
  while (i < l1) {
    if (s1[i] != s2[i]) return -2;
    i++;
  }
  return 0;
}

/**
 * Cherche le caractère donné en argument, dans la chaîne donné en argument,
 * à partir de la position donné (offset).
 */
int string_index_of(char* dst, int offset, char ch) {
  int l1;
  l1 = string_length(dst);
  while (offset < l1) {
    if (dst[offset] == ch) return offset;
    offset++;
  }
  return -1;
}

/**
 * Copie une sous-chaîne de la chaîne source dans la chaîne destination.
 * La sous-chaîne est défini par l'index du premier caractères (offset)
 * et sa longueur (length).
 * Cette sous-chaîne sera copiée dans la chaîne destination à l'index
 * donné par l'argument "doff".
 * @param dst: la chaîne destination
 * @param doff: l'offset dnas la chaîne destination
 * @param src: la chaîne source
 * @param offset: l'offset de la sous-chaîne à copier.
 * @param length: la loongueur de la sous-chaîne à copier.
 */
/*void string_substring(char* dst, int doff, char *src, int offset, int length)
{
}*/

// renvoie 1 si s2 est contenue dans s1 en partant du début;
int myStringCmp(char* s1, char* s2) {
  int l1, l2;
  l2 = string_length(s2);
  for (int i = 0; i < l2; i++) {
    if (s1[i] != s2[i]) {
      return 0;
    }
  }
  return 1;
}
