#include <stdio.h>

#include "message.h"
#include "table_skel.h"
#include "table.h"


struct table_t *table;

int table_skel_init(int n_lists){

  if ((table = table_create(n_lists)) == NULL) {
    return -1;
  }

  return 0;

}

void table_skel_destroy(){

  table_destroy(table);

}

int invoke(struct message_t *msg){


  if (table == NULL || msg == NULL) {
    return -1;
  }

  if (msg -> opcode == OP_SIZE ) {

    msg -> opcode = OP_SIZE + 1;
    msg -> c_type = CT_RESULT;
    msg -> content.result = table_size(table);

  }else if (msg -> opcode == OP_DEL) {

    if (table_del(table,msg -> content.key) == -1) {
      msg -> opcode = OP_ERROR;
    }else{
      msg -> opcode = OP_DEL + 1;
    }
    msg -> c_type = CT_NONE;
    //msg -> content.result = NULL;

  } else if (msg -> opcode == OP_GET) {

    struct data_t *d;

    if ((d = table_get(table,msg -> content.key)) == NULL) {
      msg -> opcode = OP_ERROR;
      msg -> c_type = CT_NONE;

    }else{
      msg -> opcode = OP_GET + 1;
      msg -> c_type = CT_VALUE;
    }

    msg -> content.value = d;

  } else if (msg -> opcode == OP_PUT) {

    if (table_put(table,msg -> content.entry->key, msg -> content.entry->value) == -1) {
      msg -> opcode = OP_ERROR;
    }else{
      msg -> opcode = OP_PUT + 1;
    }
    msg -> c_type = CT_NONE;
    //msg -> content.result = NULL;

  }else if (msg -> opcode == OP_GETKEYS) {

    msg -> opcode = OP_GETKEYS + 1;
    msg -> c_type = CT_KEYS;
    msg -> content.keys = table_get_keys(table);

  }

  return 0;

}
