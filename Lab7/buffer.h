#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define N 500 
#define BUFFER_SIZE 1000 

// Buffers e variáveis globais para controle
extern char buffer1[N];
extern char buffer2[BUFFER_SIZE];
extern int leitura_concluida; // Flag para indicar fim da leitura
extern int count1, count2;    // Contadores para controle de processamento

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
