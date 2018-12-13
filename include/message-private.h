#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#include "message.h"

int marsh_data(char **msg_buf, int size, int *offset, struct message_t *msg);

int marsh_key(char **msg_buf, int size, int *offset, struct message_t *msg);

int unmarsh_key(char **msg_buf, int offset, struct message_t *msg);

int unmarsh_data(char **msg_buf, int offset, struct message_t *msg);

#endif
