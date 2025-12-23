#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define PORTA 8080
#define TAMANHO_FILA 10
#define TAMANHO_BUFFER 100

int main(void) {
  int fd,clfd,n;
  int reuse = 1;
  char buffer[TAMANHO_BUFFER];
  struct sockaddr_in endereco;
  endereco.sin_family = AF_INET;
  endereco.sin_addr.s_addr = INADDR_ANY;
  endereco.sin_port = htons(PORTA);

  if ((fd = socket(endereco.sin_family, SOCK_STREAM, 0)) < 0){
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(int)) < 0){
    close(fd);
    return -1;
  }

  if (bind(fd,(struct sockaddr *)&endereco,sizeof(endereco)) < 0){
    close(fd);
    return -1;
  }

  if (listen(fd,TAMANHO_FILA) < 0){
    close(fd);
    return -1;
  }

  printf("Esperando o cliente conectar\n");

  if ((clfd = accept(fd,NULL,NULL)) < 0){
    close(fd);
    return -1;
  }

  printf("Cliente conectado!\n");
  printf("Digite apenas quando aparecer a linha \"Servidor:\"\n");

  while(buffer[0] != '@'){
    buffer[0] = '\0';
    printf("Servidor:");
    fflush(stdout);
    fflush(stdin);
    fgets(buffer,TAMANHO_BUFFER,stdin);
    send(clfd,buffer,strlen(buffer),0);
    printf("Cliente:");
    fflush(stdout);
    buffer[0] = '\0';
    n = recv(clfd,buffer,TAMANHO_BUFFER,0);
    write(STDOUT_FILENO,buffer,n);
  }

  close(fd);
  printf("A comunicacao foi encerrada.\n\n");
}
