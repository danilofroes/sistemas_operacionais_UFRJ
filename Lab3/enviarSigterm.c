//Based on  https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//O Pid de destino serah uma variavel global, para facilitar o trabalho
pid_t pidDestino;

int main(int argc, char ** argv)
{
  char pidDestinoString[10];

  //Processo obtem seu pid e imprime
  pid_t meuPid = getpid();
  printf("Meu pid eh %d \n",meuPid);

  //Processo pede o Pid do destino do sinal
  printf("Digite o pid do outro processo e aperte ENTER\n");
  fgets(pidDestinoString,9,stdin);
  pidDestino = (pid_t) atoi(pidDestinoString);
  printf("Enviarei SIGTERM para o processo %d\n",pidDestino);

  //Envia o sinal
  kill(pidDestino, SIGTERM);

  printf("SIGTERM enviado!\n");

  return 0;
}
