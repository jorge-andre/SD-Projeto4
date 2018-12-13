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
#include "table.h"
#include "table-private.h"


struct table_t *table_create(int n){

	struct table_t* table = malloc(sizeof(struct table_t));
	table -> size = n;
	table -> lista = malloc(sizeof(struct list_t*) * n);

	int i;
	for ( i = 0; i < n; i++) {
		table -> lista [i] = calloc(1,sizeof(struct list_t));
	}

	table -> nElems = 0;
	table -> nLists = 0;

	return table;
}

void table_destroy(struct table_t *table){

	 if(table != NULL){

			int i;
			for (i = 0; i < table -> size; i++){
				list_destroy(table -> lista[i]);
			}
			free(table -> lista);
		}


	free(table);
}

int table_put(struct table_t *table, char *key, struct data_t *value){

	if (table == NULL || key ==NULL || value == NULL) {
		printf("table_put\n");
		return -1;
	}

	int aux = list_add(table->lista[hash(table,key)],entry_create(strdup(key),data_dup(value)));

			if (aux == 0) {
				table -> nElems = (table -> nElems) + 1;
				if (table->lista[hash(table,key)] -> size  == 1 ) {
					table -> nLists = (table -> nLists) + 1;
				}
				return 0;
			}else if (aux == 1) {
				return 0;
			}


	return -1;

}
struct data_t *table_get(struct table_t *table, char *key){

	if (table == NULL || key == NULL || table -> nElems == 0) {
		printf("table_get\n");
		return NULL;
	}
	struct entry_t *aux = list_get(table->lista[hash(table,key)],key);

	if (aux!= NULL) {
		return data_dup( aux -> value);
	}

	return NULL;


}
int table_del(struct table_t *table, char *key){

	if (table == NULL || key == NULL ) {
		printf("table_del\n");
		return -1;
	}

		if (list_remove(table->lista[hash(table,key)],key) == 0) {
			table -> nElems= table -> nElems - 1;
			if (table->lista[hash(table,key)]->size == 0) {
				table -> nLists = (table -> nLists) - 1;
			}
			return 0;
		}


	return -1;

}

int table_size(struct table_t *table){
	if (table == NULL) {
		printf("table_size -> table esta a null\n");
		return -1;
	}

	return table -> nElems;
}

char **table_get_keys(struct table_t *table){

	if (table == NULL){
		printf("table estah a NULL -> table_get_keys\n");
		return NULL;
	}

	char** result = malloc(((table -> nElems) +1) * sizeof(char*));
	char ** array;
	int i,j,cont = 0;

	for ( i = 0; i < table -> size; i++) {
		if (table->lista[i] != NULL || table->lista[i] -> size != 0 ) {
			array = list_get_keys(table->lista[i]);

			for ( j = 0; j < table->lista[i]->size; j++) {
				result[cont] = malloc(strlen(array[j])+1);

				strcpy(result[cont],array[j]);
				cont++;
			}
			list_free_keys(array);
		}
	}

	result[cont] = NULL;
	return result;

}

void table_free_keys(char **keys){


	list_free_keys(keys);

}

int hash(struct table_t* table,char* key){
	int i,sum=0;

	for ( i = 0; i < strlen(key); i++) {
		sum = sum + key[i];
	}
	return sum % (table -> size);
}

void table_print(struct table_t *table){

	if (table -> nLists ==0) {
		printf("nao existem listas\n");
	}else{

	int i = 0 ;

	while (i < table -> nLists) {
		printf("lista %d\n",i);
		list_print(table -> lista[i]);
		printf("\n\n");
		i++;
		}
	}

}
