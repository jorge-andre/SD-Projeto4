#include <stdio.h>
#include <string.h>

#include "client_stub-private.h"
#include "client_stub.h"
#include "entry.h"
#include "data.h"
#include "message.h"

struct rtable_t *rtable_connect(const char *address_port){

  struct rtable_t *rtable = malloc(sizeof(struct rtable_t));

  char * restrict address_port_aux = malloc(strlen(address_port));

  memcpy(address_port_aux, address_port, strlen(address_port));

  rtable -> ip = strtok(address_port_aux,":");
  rtable -> port = atoi(strtok(NULL,":"));

  if (network_connect(rtable) == -1 ) {
    return NULL;
  }

  return rtable;

}

int rtable_disconnect(struct rtable_t *rtable){

  if (network_close(rtable)==-1) {
    return -1;
  }

  free(rtable);

  return 0;

}

int rtable_put(struct rtable_t *rtable, struct entry_t *entry){

  
  struct message_t *msg = create_message(OP_PUT,CT_ENTRY);

  msg -> content.entry = entry; 

  if ((msg = network_send_receive(rtable,msg))==NULL) {
    return -1;
  }

  return any_error(msg->opcode,OP_PUT); 
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){

  struct message_t *msg = create_message(OP_GET,CT_KEY);
  msg -> content.key = key;

  if ((msg = network_send_receive(rtable,msg))==NULL) {
    return NULL;
  }

  if (any_error(msg->opcode,OP_GET) != 0) {
    return NULL;
  }

  return msg -> content.value;

}

int rtable_del(struct rtable_t *rtable, char *key){

  struct message_t *msg = create_message(OP_DEL,CT_KEY);
  msg -> content.key = key;

  if ((msg = network_send_receive(rtable,msg))==NULL) {
    return -1;
  }

  return any_error(msg->opcode,OP_DEL);

}

int rtable_size(struct rtable_t *rtable){

  struct message_t *msg = create_message(OP_SIZE,CT_NONE);
  
  if ((msg = network_send_receive(rtable,msg))==NULL) {
    return -1;
  }
  
  if (any_error(msg->opcode,OP_DEL) != 0) {
    return -1;
  }

  return msg -> content.result; 

}

char **rtable_get_keys(struct rtable_t *rtable){

  struct message_t *msg = create_message(OP_GETKEYS,CT_NONE);

  if ((msg = network_send_receive(rtable,msg))==NULL) {
    return NULL;
  }

  if (any_error(msg->opcode,OP_DEL) != 0) {
    return NULL;
  }

  return msg -> content.keys;

}

void rtable_free_keys(char **keys){

  int i = 0;

  while (keys[i] != NULL) {
    free(keys[i]);
    i++;
  }

  free(keys);

}

//---------------FUNCOES PRIVATE------------------------//

struct message_t *create_message(short op,short c_tp){

  struct message_t *msg = malloc(sizeof(struct message_t));

  msg -> opcode = op;
  msg -> c_type = c_tp;

  return msg;
}

int any_error(short op_f,short op_i){

  if (op_f == OP_ERROR) {
    return -1;
  }
  return 0;
}
