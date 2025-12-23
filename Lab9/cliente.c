#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define IP_SERVIDOR "127.0.0.1"
#define PORTA_SERVIDOR 8080
#define TAMANHO_FILA 10
#define TAMANHO_BUFFER 100

int main(void) {
  int fd,n;
  char buffer[TAMANHO_BUFFER];
  struct sockaddr_in enderecoServidor;
  enderecoServidor.sin_family = AF_INET;
  enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

  if ((fd = socket(enderecoServidor.sin_family, SOCK_STREAM, 0)) < 0){
    return -1;
  }

  if (inet_pton(AF_INET, IP_SERVIDOR, &enderecoServidor.sin_addr) <= 0){
    close(fd);
    return -1;
  }

  if ((connect(fd, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor)) < 0)){
    return -1;
  }

  printf("Conectado ao servidor!\n");
  printf("Digite apenas quando aparecer a linha \"Cliente:\"\n");
  printf("Para encerrar a conexao com o servidor, envie o caracter @\n");

  while(buffer[0] != '@'){
    printf("Servidor:");
    fflush(stdout);
    buffer[0] = '\0';
    n = recv(fd,buffer,TAMANHO_BUFFER,0);
    write(STDOUT_FILENO,buffer,n);
    buffer[0] = '\0';
    printf("Cliente:");
    fflush(stdout);
    fflush(stdin);
    fgets(buffer,TAMANHO_BUFFER,stdin);
    send(fd,buffer,strlen(buffer),0);
  }

  close(fd);
}
