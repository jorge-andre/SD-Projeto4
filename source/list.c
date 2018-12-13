/* Grupo 19:
   Pedro Luis - 44826
   Andre Nicolau - 47880
   Jorge Andre - 49496
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "entry.h"
#include "list.h"
#include "list-private.h"

struct list_t *list_create(){

  struct list_t* list;
  list = malloc(sizeof(struct list_t));
  list -> size = 0;
  list -> frst = NULL;
  list -> last = NULL;

  return list;

}

void list_destroy(struct list_t *list){


  if (list != NULL) {

    if (list -> size ==1 ) {
      node_destroy(list -> frst);
    }

     if (list->size > 1) {
        struct node_t* aux;

          while (list -> frst != NULL) {

            aux = list -> frst;
            list -> frst = list -> frst -> next;
            node_destroy(aux);
          }
        }

        free(list);
    }
}


void node_destroy(struct node_t* node){
  node -> next = NULL;
  node -> prev = NULL;
  entry_destroy(node -> entry);
  free(node);
}



int list_add(struct list_t *list, struct entry_t *entry){

  if (entry == NULL) {
    printf("list_add\n");
    return -1;
  }

  if (list_get(list,entry->key) != NULL) {
    struct node_t* aux = list -> frst;

    while (aux != NULL) {
      if (strcmp(aux -> entry -> key, entry -> key)==0) {
        aux -> entry = entry;
        return 1;
      }
      aux=aux->next;
    }

  }else{

    if (list == NULL) {
    //  list = list_create();
    list -> size = 0;
    }

    struct node_t* node;
    node = malloc(sizeof(struct node_t));
    node -> entry = entry;
    node -> next = NULL;
    node -> prev = NULL;

    if (list -> size == 0) {
      list -> frst = node;
      list -> last = node;

    }else{

      list -> last -> next = node;
      node -> prev = list-> last;
      list -> last = node;

    }

    list -> size = (list -> size) + 1;

  }


  return 0;
}

int list_remove(struct list_t *list, char *key){

  if (key == NULL) {
    printf("list_remove -> key a NULL \n");
    return -1;
  }

  if (list -> frst == NULL) {
    printf("list_remove -> List esta vazia\n");
    return 0;
  }

  if (list_get(list,key) == NULL) {
    return -1;
  }

  struct node_t* aux = list -> frst;

  while (aux != NULL) {

    if (strcmp(aux -> entry -> key, key)==0) {

      if (aux == list -> last) { // ultimo
        aux -> prev -> next = NULL;
        list -> last = list -> last -> prev;
      }else if(aux == list -> frst){ // primeiro
        aux -> next -> prev = NULL;
        list -> frst = aux -> next;
      }else{
        aux -> next -> prev = aux -> prev;
        aux -> prev -> next = aux -> next;
      }

      list -> size = list -> size - 1;
      node_destroy(aux);
      return 0;
    }

  aux = aux -> next;

  }

  return -1;
}

struct entry_t *list_get(struct list_t *list, char *key){

  if (list -> size == 0 || list == NULL || key == NULL) {
    return NULL;
  }

  struct node_t* aux = list -> frst;

  while (aux != NULL) {
    if (strcmp(aux -> entry -> key,key)==0) {

      return aux -> entry;
    }
    aux = aux -> next;
  }

  return NULL;

}

int list_size(struct list_t *list){

  if (list != NULL) {
    return list -> size;
  }

  printf("Lista esta a NULL\n");
  return -1;

}

char **list_get_keys(struct list_t *list){

  if (list == NULL || list -> frst ==NULL) {
    printf("list_get_keys -> List esta a null ou nao tem elementos\n");
    return NULL;
  }

  int i;
  char** aux;
  struct node_t* node = list -> frst;

  aux = malloc(((list -> size) + 1) * sizeof(char*));

  for ( i = 0; i < list -> size; i++) {
    aux[i] = malloc(strlen(node -> entry -> key)+1);
    strcpy(aux[i],node -> entry -> key);
    node = node -> next;
  }

  return aux;
}

void list_free_keys(char **keys){

  if (keys != NULL) {

    int i = 0;

    while (keys[i] != NULL) {
      free(keys[i]);
      i++;
    }
  }
  free(keys);
}

void list_print(struct list_t* list){

  if (list -> size ==0) {
    printf("esta vazia\n");
  }

  struct node_t* aux = list -> frst;

  while (aux != NULL) {
  printf("%s\n",aux -> entry -> key);
  aux = aux -> next;
  }
  printf("tamanho: %d\n", list -> size );

}

int exist_key_l(struct list_t* list,char* key){

  if (list -> size == 0 || list == NULL || key == NULL) {
    return -1;
  }

  struct node_t* aux = list -> frst;

  while (aux != NULL) {
    if (strcmp(aux -> entry -> key,key)==0) {
      return 0;
    }
    aux = aux -> next;
  }

  return -1;
}
