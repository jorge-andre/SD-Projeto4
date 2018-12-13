#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"
#include "list-private.h"
#include "table.h"

struct table_t {
	int size;
	int nLists;
	int nElems;
	struct list_t** lista;
};

/* Função para criar/inicializar uma nova tabela hash, com n linhas
 * (módulo da função HASH).
 */
struct table_t *table_create(int n);

void table_print(struct table_t *table);

int hash(struct table_t* table,char* key);

int exist_key_table(struct table_t* table, char* key);

#endif
