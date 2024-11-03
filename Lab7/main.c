#include "buffer.h"
#include <pthread.h>
#include <stdio.h>

// Função para inicializar os semáforos
void inicializar_semaforos() {
    sem_init(&sem_buffer1_vazio, 0, 1);   // Buffer1 começa vazio
    sem_init(&sem_buffer1_pronto, 0, 0);  // Buffer1 não está pronto no início
    sem_init(&sem_buffer2_vazio, 0, 1);   // Buffer2 começa vazio
    sem_init(&sem_buffer2_pronto, 0, 0);  // Buffer2 não está pronto no início
}

// Função para destruir os semáforos ao final do programa
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

    destruir_semaforos();  // Limpa os recursos dos semáforos
    return 0;
}
