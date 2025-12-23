#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 10

int buffer[MAX];
int posicaoInsercao = 0;
int posicaoLeitura = 0;
int contador = 0;
int produtosTotais = 100;

sem_t mutex;
sem_t livre;
sem_t preenchido;

void inserirBuffer(int valor) {
    buffer[posicaoInsercao] = valor;
    posicaoInsercao++;

    if (posicaoInsercao == MAX)
        posicaoInsercao = 0;

    contador++;
}

int lerBuffer() {
    int tmp = buffer[posicaoLeitura];
    posicaoLeitura++;

    if (posicaoLeitura == MAX)
        posicaoLeitura = 0;

    contador--; 
    return tmp; 
}

void *produtor(void *arg) {
    int i;         

    for (i = 0; i < produtosTotais ; i++){
        printf("Produtor tenta lock %d\n",i);
        printf("Produtor verifica se pode produzir %d\n",i);
        sem_wait(&livre);
        printf("Produziu! %d\n",i);
        sem_wait(&mutex);
        inserirBuffer(i);
        sem_post(&mutex);
        sem_post(&preenchido);
    }

    return NULL;
}

void *consumidor(void *arg) {
    int i;

    for (i = 0; i < produtosTotais ; i++){
        printf("Consumidor tenta lock %d\n",i);
        printf("Consumidor verifica se pode consumir %d\n",i);

        sem_wait(&preenchido);
        printf("Consumiu! %d\n",i);
        sem_wait(&mutex);
        int tmp = lerBuffer();
        sem_post(&mutex);
        sem_post(&livre);

        printf("Lido: %d\n",tmp);
    }

    return NULL;
}

int main(void) {
    pthread_t prod,consu;

    sem_init(&mutex, 0, 1);
    sem_init(&livre, 0, MAX);
    sem_init(&preenchido, 0, 0);

    pthread_create(&prod, NULL, produtor, NULL); 
    pthread_create(&consu, NULL, consumidor, NULL); 
    pthread_join(prod,NULL);
    pthread_join(consu,NULL);


    sem_destroy(&mutex);
    sem_destroy(&livre);
    sem_destroy(&preenchido);

    return 0;
}