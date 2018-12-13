#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

struct node_t {
  struct entry_t* entry;
  struct node_t* next;
  struct node_t* prev;
};

struct list_t {
  int size;
  struct node_t* frst;
  struct node_t* last;
};

void list_print(struct list_t* list);

void node_destroy(struct node_t* node);

int exist_key_l(struct list_t*,char* key);

#endif
