//Based on  https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sigNumber)
{
  if (sigNumber == SIGTERM)
    printf("Recebi um SIGTERM\n");
}

int main(void)
{

  if (signal(SIGTERM, signal_handler) == SIG_ERR){
    printf("\nNao consigo tratar SIGINT\n");
  }

  //Processo obtem seu pid e imprime
  pid_t meuPid = getpid();
  printf("Meu pid eh %d \n",meuPid);

  while(1)
	sleep(1);
  return 0;
}
