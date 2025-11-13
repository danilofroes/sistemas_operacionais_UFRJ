/*
  * Adapted from the ostep book http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf
  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//Numero maximo de clientes gerados na execucao do programa
#define NR_MAX_CLIENTES_GERADOS 20
//Numero máximo de clientes que o barbeiro ira atender durante a execucao
#define NR_MAX_CLIENTES_DIA 10
//Numero de maximo de cliente que podem esperar dentro do salão
#define NR_CADEIRAS 4

int clientesAtendidos = 0;
//Contador de clientes no salao
int clientesNoSalao = 0;
//Cariável que indica que o barbeiro esta livre
int barbLivre = 0;
time_t t;

//Mutex que protege o contador de clientes no salao
pthread_mutex_t mutexContador = PTHREAD_MUTEX_INITIALIZER;
//Mutex que protege a variável que indica que o barbeiro esta livre
pthread_mutex_t mutexBarbLivre = PTHREAD_MUTEX_INITIALIZER;

//Variavel de condicao equivalente a customer do livro
pthread_cond_t temCliente = PTHREAD_COND_INITIALIZER;
//Variavel de condicao equivalente a barber do livro
pthread_cond_t barbeiroLivre = PTHREAD_COND_INITIALIZER;


void terCabeloCortado(int idCliente) {
  printf("Oi! Sou o cliente %d e estou cortando meu cabelo\n", idCliente);;
}

void cortarCabelo(void) {
  printf("Sou o barbeiro e estou cortando um cabelo!\n");
  sleep(1); //Espera 1 segundo, simulando o corte de cabelo
}

void *barbeiro(void *arg) {
  while (clientesAtendidos <= NR_MAX_CLIENTES_DIA){
    //Espera algum cliente chegar para ser atendido
    pthread_mutex_lock(&mutexContador); // lock usando o mutex da temCliente
    while (clientesNoSalao == 0) {
      printf("Barbeiro dormindo, pois nao tem cliente\n");
      pthread_cond_wait(&temCliente, &mutexContador); // vai aguardar ate o signal do temCliente chamar
    }

    printf("Barbeiro acordou\n");
    pthread_mutex_unlock(&mutexContador); // sai do lock

    //Chama um cliente
    pthread_mutex_lock(&mutexBarbLivre);
    printf("Barbeiro chamando cliente\n");
    pthread_cond_signal(&barbeiroLivre); // envia signal para a funcao wait do cliente esperando
    pthread_mutex_unlock(&mutexBarbLivre);


    //Corta cabelo
    cortarCabelo();

    printf("Terminei de cortar o cabelo de um cliente!\n");
    clientesAtendidos++;
  }
  pthread_exit(NULL);
}

void *cliente(void *arg) {
  pthread_mutex_lock(&mutexContador);
  if (clientesNoSalao == NR_CADEIRAS){
    pthread_mutex_unlock(&mutexContador);
    printf("Sou o cliente %d e fui embora por falta de cadeiras\n",*(int *)arg);
    pthread_exit(NULL);
  }
  clientesNoSalao++;
  pthread_mutex_unlock(&mutexContador);
  //Sinaliza, para o barbeiro, a existencia de um cliente
  pthread_mutex_lock(&mutexContador);
  pthread_cond_signal(&temCliente); // envia signal para a funcao wait do barbeiro dormindo
  pthread_mutex_unlock(&mutexContador);

  //Procedimento para esperar ser atendido
  pthread_mutex_lock(&mutexBarbLivre); // lock usando o mutex do barbeiroLivre
  printf("Cliente %d esperando para ser atendido\n", *(int *)arg);
  pthread_cond_wait(&barbeiroLivre, &mutexBarbLivre); // aguarda ate o signal do barbeiroLivre
  pthread_mutex_unlock(&mutexBarbLivre); // sai do lock

  //Cabelo eh cortado

  terCabeloCortado(*(int *)arg);

  //Decrementa numero de clientes no salao
  pthread_mutex_lock(&mutexContador);
  clientesNoSalao--;
  pthread_mutex_unlock(&mutexContador);
  pthread_exit(NULL);
}

int main(void) {
  //Threads dos clientes
  pthread_t threadsCliente[NR_MAX_CLIENTES_GERADOS];
  //Vetor para armazenar os ids dos clientes
  int idsClientes[NR_MAX_CLIENTES_GERADOS];
  //Thread do barbeiro
  pthread_t threadBarbeiro;
  //Inicializa semente aleatoria
  srand((unsigned) time(&t));
  //Cria a thread do barbeiro
  pthread_create(&threadBarbeiro, NULL, barbeiro, NULL);
  //Gera os clientes. O intervalo de geracao de clientes eh aleatorio.
  for (int i = 0 ; i < NR_MAX_CLIENTES_GERADOS ; i++){
    //Espera um tempo aleatorio de ate 3 segundos
    sleep(rand() % 4);
    idsClientes[i] = i;
    pthread_create(&threadsCliente[i], NULL, cliente, &idsClientes[i]);
  }
  //Programa soh termina quando barbeiro atendeu NR_MAX_CLIENTES_DIA
  //Note que programa pode nunca termina se NR_MAX_CLIENTES_GERADOS for baixo
  pthread_join(threadBarbeiro,NULL);
  return 0;
}
