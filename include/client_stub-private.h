#include "message.h"
#include "data.h"
#include "entry.h"
#include "network_client.h"

struct rtable_t{

  char *ip;
  int port;
  int sockfd;

};

struct message_t *create_message(short op,short c_tp);

int any_error(short op_f,short op_i );
