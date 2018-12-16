#include <stdio.h>
#include <pthread.h>

#include "network_server.h"
#include "network_server-private.h"

int sockfd;

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar 0 (OK) ou -1 (erro).
 */
struct sockaddr_in server;
struct sockaddr_in client;
socklen_t size_client;
pthread_mutex_t mtx;

struct thread_parametros{
  int socket;
};

int network_server_init(short port){


	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("Falha a criar o socket\n");

		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd,(struct sockaddr *) &server,sizeof(server))<0) {
    printf("socket fechado1\n");
		close(sockfd);
		return -1;
	}

	if (listen(sockfd,0)<0) {
    printf("socket fechado2\n");
		close(sockfd);
		return -1;
	}

	return sockfd;

}

/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket){

    int socket_de_cliente;
    int i = 0;
    pthread_t *threads = malloc(sizeof(long));
    struct thread_parametros tp;

    pthread_mutex_init(&mtx, NULL);

	while((socket_de_cliente = accept(listening_socket,(struct sockaddr * restrict) &client, &size_client)) != -1){

      /*int aux;
      void *buf = malloc(4);
      int len = 2;
      while((aux = recv(socket_de_cliente,buf,len,MSG_PEEK))!=0){

      if(aux != -1){
*/  
        threads = realloc(threads, sizeof(threads) + sizeof(long));
        i++;
        tp.socket = socket_de_cliente;

        if(pthread_create(&threads[i], NULL, connection_handler, (void *) &tp) < 0){
            printf("Erro criar thread\n");
            return -1;
        }

        pthread_join(threads[i], NULL);

	}
  free(threads);


	if (socket_de_cliente < 0) {
		close(socket_de_cliente);
        return -1;
	}

  return 0;
}

void *connection_handler(void *params){
    struct thread_parametros *tp = (struct thread_parametros *) params;
    int socket = tp->socket;
	struct message_t  *msg;

    while ((msg = network_receive(socket)) != NULL){ // ate haver msg para ler
 
        pthread_mutex_lock(&mtx);
		if (invoke(msg)<0) {
            close(socket);
			return -1;
		}
        pthread_mutex_unlock(&mtx);


		if (network_send(socket,msg) < 0) {
			close(socket);
            return -1;
		}
    }

    return 0;
}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura message_t.
 */
struct message_t *network_receive(int client_socket){

	int size = OPCODE_SIZE + C_TYPE_SIZE;
  char *buf = malloc(size);

  int nbytes;
  short c_type_net;
  short c_type;
  
  if ((nbytes=read_all(client_socket,buf,OPCODE_SIZE)) != OPCODE_SIZE) {

    printf("socket fechado\n");
    close(client_socket);
    free(buf);
    return NULL;
  }

  if ((nbytes=read_all(client_socket,(buf+OPCODE_SIZE),C_TYPE_SIZE))!=C_TYPE_SIZE) {
    printf("socket fechado\n");
    close(client_socket);
    free(buf);
    return NULL;
  }

  memcpy(&c_type_net,buf+OPCODE_SIZE,C_TYPE_SIZE);
  c_type = ntohs(c_type_net);

  

  switch (c_type) {
    case CT_KEY :{

      short s_k,s_k_net;

      if ((nbytes=read_all(client_socket,&s_k_net,2))!=2) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      s_k = ntohs(s_k_net);

      buf = realloc(buf,size + 2 );
      memcpy(buf+size,&s_k_net,2);
      size +=2;

      buf = realloc(buf,size + 2 + s_k );

      if ((nbytes=read_all(client_socket,buf + size,s_k))!=s_k) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      size += s_k;


      break;
    }case CT_ENTRY:{
        
        int s_d,s_d_net;
        short s_k,s_k_net;

      if ((nbytes=read_all(client_socket,&s_k_net,2))!=2) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      s_k = ntohs(s_k_net);

      buf = realloc(buf,size + 2 );
      memcpy(buf+size,&s_k_net,2);
      size +=2;

      buf = realloc(buf,size + 2 + s_k );

      if ((nbytes=read_all(client_socket,buf + size,s_k))!=s_k) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      size += s_k;

      if ((nbytes=read_all(client_socket,&s_d_net,4))!=4) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      s_d = ntohl(s_d_net);

      buf = realloc(buf,size+4);
      memcpy(buf+size,&s_d_net,4);
      size += 4;

      buf = realloc(buf,size+s_d);

      
      if ((nbytes=read_all(client_socket,buf+size,s_d))!=s_d) {
        printf("socket fechado\n");
        close(client_socket);
        free(buf);
        return NULL;
      }

      size += s_d;

      
      break;
    }
  }
  
  return buffer_to_message(buf,size);

}

/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, struct message_t *msg){

	if (msg == NULL) {
		return -1;
	}

	int nbytes;
	char *buf;
  int size_b;
	if ((size_b = message_to_buffer(msg,&buf)) < 0) {
		return -1;
	}
	free_message(msg);


	if ((nbytes = write_all(client_socket, buf , size_b)) < size_b) {
        printf("socket fechado\n");
		close(client_socket);
		free(buf);
		return -1;
	}
	return 0;

}

/* A função network_server_close() fecha a ligação estabelecida por
 * network_server_init().
 */
int network_server_close(){

	close(sockfd);
  return 0;

}
