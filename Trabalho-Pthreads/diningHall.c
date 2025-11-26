/*
 * Trabalho de Sistemas Operacionais - Uso de Locks e Variaveis de Condicao
 * 
 * Grupo 20
 * Nomes: Danilo Froes e Beatriz Lima
 * Professor: Rodrigo
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUMERO_ARGUMENTOS 2

typedef struct {
    int pessoasComendo;          // Pessoas comendo
    int pessoasAguardandoMesa;    // Pessoas com bandeja na mão
    int pessoasAguardandoSaida; // Pessoas que acabaram e querem sair
    int estudantesAtivos;
    
    pthread_mutex_t mutex;
    pthread_cond_t condEstado; // CV única para simplificar (broadcast)
} DiningHall;

typedef struct {
    int id;
    int numeroRefeicoes;
    DiningHall *dh;
} ArgsThread;

void pegarComida(DiningHall *dh) {
    pthread_mutex_lock(&dh->mutex);
    dh->pessoasAguardandoMesa++;
    // Avisa quem está travado na saída que chegou alguém novo!
    pthread_cond_broadcast(&dh->condEstado); 
    pthread_mutex_unlock(&dh->mutex);
}

void prontoParaJantar(DiningHall *dh) {
    pthread_mutex_lock(&dh->mutex);

    // Enquanto (Mesa Vazia E Eu sou o único esperando)
    while (dh->pessoasComendo == 0 && dh->pessoasAguardandoMesa < 2 && dh->estudantesAtivos > 1) {
        pthread_cond_wait(&dh->condEstado, &dh->mutex);
    }

    // Transição de estado: Deixou de ser "ready", passou a ser "pessoasComendo"
    dh->pessoasAguardandoMesa--;
    dh->pessoasComendo++;
    
    // Avisa o mundo (pode destravar alguém esperando para comer ou sair)
    pthread_cond_broadcast(&dh->condEstado);
    pthread_mutex_unlock(&dh->mutex);
}

void sairMesa(DiningHall *dh) {
    pthread_mutex_lock(&dh->mutex);
    
    dh->pessoasAguardandoSaida++; // Declaro minha intenção de sair

    // A condição de BLOQUEIO (quando eu NÃO posso sair)
    while (dh->pessoasComendo == 2 && dh->pessoasAguardandoMesa == 0 && dh->pessoasAguardandoSaida < 2) {
        pthread_cond_wait(&dh->condEstado, &dh->mutex);
    }

    // Procedo com a saída
    dh->pessoasAguardandoSaida--; // Já não estou tentando, estou saindo
    dh->pessoasComendo--;

    pthread_cond_broadcast(&dh->condEstado);
    pthread_mutex_unlock(&dh->mutex);
}

void inicializarJantar(DiningHall *dh, int totalThreads) {
    dh->pessoasComendo = 0;
    dh->pessoasAguardandoMesa = 0;
    dh->pessoasAguardandoSaida = 0;
    dh->estudantesAtivos = totalThreads;

    pthread_mutex_init(&dh->mutex, NULL);
    pthread_cond_init(&dh->condEstado, NULL);
}

void *rotinaCliente(void *args) {
    ArgsThread *tArgs = (ArgsThread*) args;
    DiningHall *dh = tArgs->dh;
    int id = tArgs->id;
    int maxRefeicoes = tArgs->numeroRefeicoes;

    for (int i = 0; i < maxRefeicoes; i++) {
        printf("[Cliente %d] Pegou bandeja (Iteracao %d/%d)\n", id, i+1, maxRefeicoes);
        pegarComida(dh);

        printf("[Cliente %d] Esperando para sentar...\n", id);
        prontoParaJantar(dh);

        printf("[Cliente %d] SENTOU e esta comendo.\n", id);

        usleep((rand() % 1000) * 1000); // simula tempo comendo

        printf("[Cliente %d] Terminou. Tentando sair...\n", id);
        sairMesa(dh);
        
        printf("[Cliente %d] SAIU da mesa.\n", id);
    }

    pthread_mutex_lock(&dh->mutex);
    dh->estudantesAtivos--;
    
    pthread_cond_broadcast(&dh->condEstado); // acorda quem sobrou
    pthread_mutex_unlock(&dh->mutex);

    printf("[Cliente %d] Foi embora.\n", id);

    free(tArgs); // liberando a memoria que foi alocada

    return NULL;
}

int main(int argc, char *argv[]) {
    int numeroThreads;
    int numeroRefeicoes;
    char *validacaoNumeroThreads;
    char *validacaoNumeroRefeicoes;
    DiningHall dh; // instanciando o refeitorio

    // verificacao para chamada do codigo
    if (argc != NUMERO_ARGUMENTOS + 1) {
        printf("\nUso: %s <numero_threads> <numero_refeicoes>.\n\n", argv[0]);
        exit(1);
    }

    numeroThreads = strtol(argv[1], &validacaoNumeroThreads, 10);
    // verificacao para a conversao do argumento de entrada
    if (*validacaoNumeroThreads != '\0') {
        printf("\nO argumento contem um caractere invalido (%c).\n\n", *validacaoNumeroThreads);
        exit(1);
    }

    numeroRefeicoes = strtol(argv[2], &validacaoNumeroRefeicoes, 10);
    if (*validacaoNumeroRefeicoes != '\0') {
        printf("\nO argumento contem um caractere invalido (%c).\n\n", *validacaoNumeroRefeicoes);
        exit(1);
    }

    printf("\nIniciando simulacao com %d threads e %d refeicoes cada.\n\n", numeroThreads, numeroRefeicoes);

    // inicializacao
    inicializarJantar(&dh, numeroThreads);
    srand((unsigned) time(NULL)); // semente aleatoria
    pthread_t threadsClientes[numeroThreads];

    // criacao das threads
    for (int i = 0; i < numeroThreads; i++) {
        ArgsThread *args = malloc(sizeof(ArgsThread)); // alocando memoria para passar os argumentos para cada thread
        args->id = i;
        args->numeroRefeicoes = numeroRefeicoes;
        args->dh = &dh;

        // verificacao de erro
        if (pthread_create(&threadsClientes[i], NULL, rotinaCliente, (void*)args) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    for (int i = 0; i < numeroThreads; i++) {
        pthread_join(threadsClientes[i], NULL);
    }

    printf("\nTodos os clientes foram embora.\n Restaurante fechado.\n");

    pthread_mutex_destroy(&dh.mutex);
    pthread_cond_destroy(&dh.condEstado);

    return 0;
}