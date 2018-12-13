/*
	Programa cliente para manipular tabela de hash remota.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "network_client.h"
#include "client_stub.h"
#include "entry.h"
#include "data.h"
#include "table-client.h"

int COM_SPACE = 200;
int KEY_SPACE = 50;
int DATA_SPACE = 150;

int main(int argc, char **argv){

	int program_output = 0;

	/* Testar os argumentos de entrada */
	switch(argc){
		case 2:
			program_output = normal_cicle(argv[1]);
			break;

		case 5:
			if(strcmp(argv[2], "p") == 0){
				program_output = put_cicle(argv[1], argv[3]);
			} else if(strcmp(argv[2], "g") == 0){
				program_output = get_cicle(argv[1], argv[3]);
			} else{
				program_output = -1;
			}
			break;

		default:
			printf("Numero de argumentos incorrecto.");
			break;
	}

	return program_output;
	
	
}

int put_cicle(char *address_port, char *secs){

	struct rtable_t *rtable;
	char *key = malloc(KEY_SPACE);
	char *data = malloc(DATA_SPACE);
	int seconds = atoi(secs);
	time_t start, stop;
	data = "put automatico";
	int key_number = 1;

	rtable = rtable_connect(address_port);

	if(rtable==NULL){
		printf("Erro na ligaçao ao servidor\n");	
		exit(-1);
	}
	start = time(NULL);
	stop = time(NULL);

	while(stop - start < seconds){
		sprintf(key, "%d", key_number);

		if(rtable_put(rtable, entry_create(key, data_create2(strlen(data), data))) != 0){
			printf("Erro ao adicionar.\n");
		}
		key_number++;
		stop = time(NULL);
	}

	printf("n puts: %d\n", atoi(key));

	return rtable_disconnect(rtable);
}

int get_cicle(char *address_port, char *secs){

	struct rtable_t *rtable;
	char *key = malloc(KEY_SPACE);
	int seconds = atoi(secs);
	time_t start, stop;
	int key_number = 1;

	rtable = rtable_connect(address_port);

	if(rtable==NULL){
		printf("Erro na ligaçao ao servidor\n");	
		exit(-1);
	}
	start = time(NULL);
	stop = time(NULL);

	while(stop - start < seconds){
		sprintf(key, "%d", key_number);

		if(rtable_get(rtable, key) != 0){
			printf("Erro ao fazer o get.\n");
		}
		key_number++;
		stop = time(NULL);
	}

	printf("n gets: %d\n", atoi(key));

	return rtable_disconnect(rtable);
}

/* Realiza o ciclo comum do cliente: recebe input, envia o comando, recebe resultado. Até o utilizador fizer quit */
int normal_cicle(char *address_port){

	struct rtable_t *rtable;
	char *input = malloc(COM_SPACE);
	char *key = malloc(KEY_SPACE);
	char *data = malloc(DATA_SPACE);

	/* Iniciar instância do stub e Usar rtable_connect para estabelcer ligação ao servidor*/

	rtable = rtable_connect(address_port);

	
	if(rtable==NULL){
		printf("Erro na ligaçao ao servidor\n");	
		exit(-1);
	}

	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
 	while (1){

		printf(">>> "); // Mostrar a prompt para inserção de comando

		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		*/
		fgets(input,COM_SPACE,stdin);
		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		*/
		
		input[strlen(input)-1] = '\0';
		
		char *aux = strtok(input," ");
		

		if (strcmp(aux,"quit") == 0) {

			
			//free(aux);
			//free(input);
			//free(key);
			//free(data);
			return rtable_disconnect(rtable);
		}

		/* Caso contrário:
			Verificar qual o comando;
			chamar função do stub para processar o comando e receber msg_resposta
		*/
		
		
		if (strcmp(aux,"put") == 0) {
			key = strtok(NULL," ");
			data = strtok(NULL," ");
			if (rtable_put(rtable,entry_create(key,data_create2(strlen(data),data)))==0) {
				printf("Adicionado com sucesso!\n");
			}else{
				printf("Erro ao Adicionar\n");
			}
		}else if (strcmp(aux,"get")== 0) {
			key = strtok(NULL," ");
			struct data_t *d;
			if ((d = rtable_get(rtable,key))==NULL) {
				printf("Erro ao fazer o get\n");
			}else{
				printf("data : %s\n",(char*)d->data);
				data_destroy(d);
			}
		}else if (strcmp(aux,"del")== 0) {
			key = strtok(NULL," ");
			if (rtable_del(rtable,key) == 0) {
				printf("Eliminado com sucesso!\n");
			}else{
				printf("Erro ao Eliminar\n");
			}
		}else if (strcmp(aux,"size")== 0) {
			int s;
			
			if ((s=rtable_size(rtable)) < 0) {
				printf("Erro ao solicitar o tamanho\n");
			}else{
				printf("size: %d\n",s);
			}
		}else if (strcmp(aux,"getkeys")== 0) {
			char ** ks;
			if ((ks=rtable_get_keys(rtable))==NULL) {
				printf("Erro a reunir as chaves\n");
			}else{
				
				int i = 0;
				while (ks[i] != NULL) {
					printf("%s\n",ks[i]);
					i++;
				}
				rtable_free_keys(ks);
			}
		}else{
			printf("Comando invalido\n");
		}

	}
  	return rtable_disconnect(rtable);
}
