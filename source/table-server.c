/*
   Programa que implementa um servidor de uma tabela hash com chainning.
   Uso: table-server <port> <n_lists>
   Exemplo de uso: ./table_server 54321 6
*/
#include <error.h>

#include "network_server.h"
#include "table_skel.h"


int main(int argc, char **argv){

	/* Testar os argumentos de entrada */

	if (argc != 3) {
		printf("Uso: table_server <port> <n_lists>\n");
		printf("Exemplo de uso: ./table_server 54321 6\n");
		exit(-1);
	}
	//talvez testar os argumentos
	
	/* inicialização da camada de rede */
	int socket_de_escuta = network_server_init((short)atoi(argv[1]));

	table_skel_init(atoi(argv[2]));

	int result = network_main_loop(socket_de_escuta);
	
	if(result<0){
		printf("acabou com erro\n");
	}
	
	table_skel_destroy();
}






	/**************************************************************************/
	/* socket_de_escuta = socket(…);                                          */
	/* bind(socket_de_escuta, porto obtido na linha de comando);              */
	/* listen(socket_de_escuta);                                              */
	/**************************************************************************/
//	if ((listening_socket = make_server(atoi(argv[1]))) < 0) return -1;

	/**************************************************************************/
	/* Criar a tabela hash de acordo com n_lists passada na linha de comandos */
	/**************************************************************************/
// 	table_skel_init();
//
// 	while((socket_de_cliente = accept(listening_socket) != -1)){
// 		if (socket_de_cliente tem dados para ler) {
// 			nbytes = read_all(socket_de_cliente, buffer, …);
// 			if(read returns 0 bytes) {
// 				/* sinal de que a conexão foi fechada pelo cliente */
// 				close(socket_de_cliente);
// 			} else {/* processamento da requisição e da resposta */
// 				message = buffer_to_message(buffer);
// 				msg_out = invoke(message);
// 				buffer = message_to_buffer(msg_out);
// 				write_all(socket_de_cliente, buffer, …);
// 			}
// 		}
//
// 		if (socket_de_cliente com erro) {
// 			close(socket_de_cliente);
// 		}
// 	}
//
// 	table_skel_destroy();
//
// }
