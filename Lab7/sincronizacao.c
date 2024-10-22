#include "sincronizacao.h"

// Inicialização dos semáforos e mutex
sem_t sem_buffer1_vazio, sem_buffer2_vazio;
sem_t sem_buffer1_pronto, sem_buffer2_pronto;
pthread_mutex_t mutex;

void iniciar_sincronizacao() {
    sem_init(&sem_buffer1_vazio, 0, 1);
    sem_init(&sem_buffer2_vazio, 0, 1);
    sem_init(&sem_buffer1_pronto, 0, 0);
    sem_init(&sem_buffer2_pronto, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}

void finalizar_sincronizacao() {
    sem_destroy(&sem_buffer1_vazio);
    sem_destroy(&sem_buffer2_vazio);
    sem_destroy(&sem_buffer1_pronto);
    sem_destroy(&sem_buffer2_pronto);
    pthread_mutex_destroy(&mutex);
}
