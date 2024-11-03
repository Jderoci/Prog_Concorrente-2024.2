#include "buffer.h"
#include <pthread.h>
#include <stdio.h>

// Inicializa e destrói os semáforos
void inicializar_semaforos() {
    sem_init(&sem_buffer1_vazio, 0, 1);
    sem_init(&sem_buffer1_pronto, 0, 0);
    sem_init(&sem_buffer2_vazio, 0, 1);
    sem_init(&sem_buffer2_pronto, 0, 0);
}

void destruir_semaforos() {
    sem_destroy(&sem_buffer1_vazio);
    sem_destroy(&sem_buffer1_pronto);
    sem_destroy(&sem_buffer2_vazio);
    sem_destroy(&sem_buffer2_pronto);
}

int main() {
    pthread_t threads[3];

    inicializar_semaforos();

    // Cria as threads de leitura, processamento e impressão
    pthread_create(&threads[0], NULL, leitura, NULL);
    pthread_create(&threads[1], NULL, processamento, NULL);
    pthread_create(&threads[2], NULL, impressao, NULL);

    // Espera todas as threads finalizarem
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    destruir_semaforos();
    return 0;
}
