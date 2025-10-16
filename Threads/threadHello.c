#include <stdio.h>
#include <pthread.h>

void * funcaoImprime(void *arg){
    printf("%s\n", (char *) arg);
    pthread_exit(NULL);
}


int main(void) {
	pthread_t p1, p2;

	printf("\nCriando threads\n\n");

	//Cria as threads
	pthread_create(&p1,NULL,funcaoImprime,"A");
	pthread_create(&p2,NULL,funcaoImprime,"B");

	//Espera
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("Terminando!\n");
	
	return 0;
}


