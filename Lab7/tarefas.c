#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "sincronizacao.h"

#define N 500
#define BUFFER_SIZE 1000

// Buffers e variáveis de controle
char buffer1[N];
char buffer2[BUFFER_SIZE];
int leitura_concluida = 0;
int count1 = 0, count2 = 0;

void *leitura(void *arg) {
    FILE *file = fopen("entrada.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&sem_buffer1_vazio);

        count1 = fread(buffer1, sizeof(char), N - 1, file);
        if (count1 <= 0) {
            pthread_mutex_lock(&mutex);
            leitura_concluida = 1;
            pthread_mutex_unlock(&mutex);

            sem_post(&sem_buffer1_pronto);
            break;
        }
        buffer1[count1] = '\0';
        sem_post(&sem_buffer1_pronto);
    }

    fclose(file);
    pthread_exit(NULL);
}

void *processamento(void *arg) {
    int n = 0, cont = 0;

    while (1) {
        sem_wait(&sem_buffer1_pronto);
        sem_wait(&sem_buffer2_vazio);

        pthread_mutex_lock(&mutex);
        if (leitura_concluida && count1 == 0) {
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_buffer2_pronto);
            break;
        }
        pthread_mutex_unlock(&mutex);

        for (int i = 0; i < count1; i++) {
            buffer2[count2++] = buffer1[i];
            cont++;

            if ((cont == (2 * n + 1) && n < 10) || (cont == 10 && n >= 10)) {
                buffer2[count2++] = '\n';
                n = (n < 10) ? n + 1 : n;
                cont = 0;
            }
        }
        buffer2[count2] = '\0';
        sem_post(&sem_buffer2_pronto);
    }

    pthread_exit(NULL);
}

void *impressao(void *arg) {
    while (1) {
        sem_wait(&sem_buffer2_pronto);

        pthread_mutex_lock(&mutex);
        if (leitura_concluida && buffer2[0] == '\0') {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        printf("%s", buffer2);
        count2 = 0;
        sem_post(&sem_buffer1_vazio);
    }

    pthread_exit(NULL);
}
