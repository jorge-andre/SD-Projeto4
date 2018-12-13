/* Grupo 19:
   Pedro Luis - 44826
   Andre Nicolau - 47880
   Jorge Andre - 49496
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


struct data_t *data_create(int size){

  if (size <= 0) {
    return NULL;
  }

  struct data_t* dataNew;
  dataNew = malloc(sizeof(struct data_t));
  dataNew -> datasize = size;
  dataNew -> data = malloc(size);

  return dataNew;
}

struct data_t *data_create2(int size, void *data){


  if (size <= 0 || data == NULL ) {
    return NULL;
  }

  struct data_t* dataNew;
  dataNew = malloc(sizeof(struct data_t));
  dataNew -> datasize = size;
  dataNew -> data = data;

  return dataNew;

}

void data_destroy(struct data_t *data){

  if (data != NULL) {
    free(data->data);
    free(data);
  }
}

struct data_t *data_dup(struct data_t *data){

  if (data == NULL || data -> datasize <= 0 || data -> data == NULL) {
    return NULL;
  }

  struct data_t* dataDup;
  dataDup = malloc(sizeof(struct data_t));
  dataDup -> datasize = data -> datasize;
  dataDup -> data = malloc(data -> datasize);
  memcpy(dataDup->data,data->data,data->datasize);


  return dataDup;


}
