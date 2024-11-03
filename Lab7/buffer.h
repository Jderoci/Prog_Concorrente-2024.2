#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define N 500              
#define BUFFER_SIZE 1000

// Buffers e variáveis globais
extern char buffer1[N];
extern char buffer2[BUFFER_SIZE];

// Semáforos
extern sem_t sem_buffer1_vazio, sem_buffer2_vazio;
extern sem_t sem_buffer1_pronto, sem_buffer2_pronto;

// Funções para inicializar/destruir semáforos e as tarefas das threads
void inicializar_semaforos();
void destruir_semaforos();
void *leitura(void *arg);
void *processamento(void *arg);
void *impressao(void *arg);

#endif
