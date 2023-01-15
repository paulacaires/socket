/*
 * cliente.c
 * 
 * Murillo Justino dos Santos 
 * Paula Caires Silva
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/* Campos de tamanho variável (tamanho máximo) */
#define TAM_MAX_BUFFER 128

/* Mensagens padrões */
#define ERRO_SOCKET_NAO_CRIADO  	 "ERRO: Socket não criado.\n"
#define SOCKET_CRIADO 		    	 "Socket criado com sucesso.\n"
#define SOCKET_FECHADO 		    	 "Socket fechado com sucesso.\n"
#define ERRO_CONEXAO_NAO_CRIADA 	 "ERRO: Conexão não criada.\n"
#define CONEXAO_CRIADA               "Conexão criada com sucesso.\n"
#define ERRO_MENSAGEM_NAO_RECEBIDA   "ERRO: O cliente não recebeu a mensagem.\n"
#define MENSAGEM_RECEBIDA   		 "O servidor enviou essa mensagem ao cliente %s\n"
#define ERRO_MENSAGEM_NAO_ENVIADA    "ERRO: O cliente não enviou a mensagem.\n"
#define MENSAGEM_ENVIADA   		 	 "O cliente enviou essa mensagem ao servidor: %s\n"
#define ERRO_ABERTURA_ARQUIVO	     "ERRO: O arquivo não pode ser aberto\n"
#define ARQUIVO_ABERTO 		 	 	 "Arquivo aberto com sucesso.\n"

int criar_socket();

int main(int argc, char **argv)
{
	char * ip = "127.0.0.1";
	unsigned port = 1903;

	// Criar o socket
	int s = criar_socket();

	/*
	 * Estrutura para o endereço de rede.
	 * Da biblioteca <netinet/in.h>
	 * */
	struct sockaddr_in endereco_server;
	endereco_server.sin_family = AF_INET;
	endereco_server.sin_port = htons(port);
	endereco_server.sin_addr.s_addr = inet_addr(ip);
	
	// Enviar a conexão para o servidor
	if(connect(s, (struct sockaddr*)&endereco_server, sizeof(endereco_server)) < 0){
		printf(ERRO_CONEXAO_NAO_CRIADA);
		return (-1);
	}
	printf(CONEXAO_CRIADA);

	int opcao;	
	char nomeArq[1000], opcao_text[2];
	printf("O que você deseja fazer?\n");
	printf("[1] enviar arquivo\n");
	printf("[2] receber arquivo\n");
	
	scanf(" %d", &opcao);
	sprintf(opcao_text, "%d", opcao);
	
	if(send(s, opcao_text, 1, 0) < 0){
		printf(ERRO_MENSAGEM_NAO_ENVIADA);
		return (-1);
	}
	
	switch (opcao) {
		case 1:
			// Manda o nome do arquivo
			printf("Qual o nome do arquivo?: ");
			scanf(" %[^\n]", nomeArq);
			
			if(send(s, nomeArq, strlen(nomeArq), 0) < 0){
				printf(ERRO_MENSAGEM_NAO_ENVIADA);
				return (-1);
			}

			// Mandar arquivo para o servidor;
			FILE *fp_cliente;
			if (( fp_cliente = fopen (nomeArq,"r") ) == NULL) {
				printf (ERRO_ABERTURA_ARQUIVO);
				return (-1);
			}
			printf(ARQUIVO_ABERTO);

			char buffer_cliente[TAM_MAX_BUFFER];
			fgets(buffer_cliente, TAM_MAX_BUFFER, fp_cliente);

			// Enviar a mensagem para o servidor
			if(send(s, buffer_cliente, strlen(buffer_cliente), 0) < 0){
				printf(ERRO_MENSAGEM_NAO_ENVIADA);
				return (-1);
			}

			printf(MENSAGEM_ENVIADA, buffer_cliente);
			fclose(fp_cliente);
			break;
		case 2:
			// Manda o nome do arquivo
			printf("Qual o nome do arquivo?: ");
			scanf(" %[^\n]", nomeArq);

			if(send(s, nomeArq, strlen(nomeArq), 0) < 0){
				printf(ERRO_MENSAGEM_NAO_ENVIADA);
				return (-1);
			}
			// Receber o arquivo do servidor.
			char buffer_servidor[TAM_MAX_BUFFER];

			if(recv(s, buffer_servidor, sizeof(buffer_servidor), 0) < 0){
				printf(ERRO_MENSAGEM_NAO_RECEBIDA);
				return (-1);
			}

			printf(MENSAGEM_RECEBIDA, buffer_servidor);
			break;
		default:
			printf("Opção inválida.\n");
			return (-1);
	}

	close(s);
	printf(SOCKET_FECHADO);
	return 0;
}

int criar_socket() {		
	// AF_INET IPv-4
	// SOCK_STREAM Conexão orientada ao protocolo TCP
	int s = socket(AF_INET, SOCK_STREAM, 0);
	
	if (s < 0) {
		printf(ERRO_SOCKET_NAO_CRIADO);
		exit(0);
	}

	printf(SOCKET_CRIADO);
	return(s);	
}

