#ifndef SINCRONIZACAO_H
#define SINCRONIZACAO_H

#include <pthread.h>
#include <semaphore.h>

// Declaração dos semáforos e mutex
extern sem_t sem_buffer1_vazio, sem_buffer2_vazio;
extern sem_t sem_buffer1_pronto, sem_buffer2_pronto;
extern pthread_mutex_t mutex;

// Funções para inicialização e finalização
void iniciar_sincronizacao();
void finalizar_sincronizacao();

#endif // SINCRONIZACAO_H
