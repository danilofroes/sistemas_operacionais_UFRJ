#include <stdio.h>
#include <pthread.h>

int contador = 0;

void *alterarContador(void *arg) {
    unsigned int i;

    for (i = 0; i < 10000000; i++) {
	    contador = contador + 1;
    }

    pthread_exit(NULL);
}
                                                                             
int main(void) {                    
    pthread_t p1, p2;
    
    pthread_create(&p1, NULL, alterarContador, NULL);
    pthread_create(&p2, NULL, alterarContador, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Valor do contador: %d\n", contador);

    return 0;
}

