#ifndef _TABLE_CLIENT_H
#define _TABLE_CLIENT_H

int normal_cicle(char *address_port);

int put_cicle(char *address_port);

int get_cicle(char *address_port);

void *thread_put(void *params);

void *thread_get(void *params);

#endif