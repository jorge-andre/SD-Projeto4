/* Grupo 19:
   Pedro Luis - 44826
   Andre Nicolau - 47880
   Jorge Andre - 49496
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "data.h"

struct entry_t *entry_create(char *key, struct data_t *data){

  if (key == NULL || data ==NULL) {
    return NULL;
  }

  struct entry_t* novaE;
  novaE = malloc(sizeof(struct entry_t));
  novaE -> key = key;
  novaE -> value = data;

  return novaE;
}

void entry_initialize(struct entry_t *entry){

  if (entry != NULL) {
    entry -> key = NULL;
    entry -> value = NULL;
  }

}

void entry_destroy(struct entry_t *entry){

  if (entry != NULL) {
    free(entry -> key);
    data_destroy(entry -> value);
    free(entry);
  }

}

struct entry_t *entry_dup(struct entry_t *entry){

  if (entry == NULL) {
    return NULL;
  }

  struct entry_t* dup;
  dup = malloc(sizeof(struct entry_t));
  //dup -> key = malloc(strlen(entry -> key)+1);
  //strcpy(entry->key,dup->key);
  dup -> key = strdup(entry -> key);
  dup -> value = data_dup(entry -> value);

  return dup;


}
