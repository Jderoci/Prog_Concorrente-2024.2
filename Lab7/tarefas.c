#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer1[N];
char buffer2[BUFFER_SIZE];

// Função da thread 1
void *leitura(void *arg) {
    FILE *file = fopen("entrada.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&sem_buffer1_vazio);

        if (fgets(buffer1, N, file) == NULL) {
            buffer1[0] = EOF;  // Marca o fim do arquivo
            sem_post(&sem_buffer1_pronto);
            break;
        }

        sem_post(&sem_buffer1_pronto);  // Sinaliza que buffer1 está pronto
    }

    fclose(file);
    pthread_exit(NULL);
}

// Função da thread 2
void *processamento(void *arg) {
    int input_index, output_index;
    int bloco_contador = 0;
    int char_contador;

    while (1) {
        sem_wait(&sem_buffer1_pronto);
        sem_wait(&sem_buffer2_vazio);

        if (buffer1[0] == EOF) {
            buffer2[0] = EOF;  // Marca fim de dados em buffer2
            sem_post(&sem_buffer2_pronto);
            break;
        }

        int buffer1_size = strlen(buffer1);
        input_index = output_index = char_contador = 0;

        // Copia dados de buffer1 para buffer2 em blocos
        while (input_index < buffer1_size) {
            int bloco_tamanho = (bloco_contador <= 10) ? (2 * bloco_contador + 1) : 10;

            for (; char_contador < bloco_tamanho && input_index < buffer1_size; char_contador++) {
                if (output_index < BUFFER_SIZE - 1) {
                    buffer2[output_index++] = buffer1[input_index++];
                } else {
                    break;
                }
            }

            if (char_contador == bloco_tamanho) {  // Adiciona nova linha após cada bloco
                if (output_index < BUFFER_SIZE - 1) {
                    buffer2[output_index++] = '\n';
                }
                char_contador = 0;
                bloco_contador++;
            }
        }

        buffer2[output_index] = '\0';
        sem_post(&sem_buffer1_vazio);
        sem_post(&sem_buffer2_pronto);
    }

    pthread_exit(NULL);
}

// Função da thread 3
void *impressao(void *arg) {
    while (1) {
        sem_wait(&sem_buffer2_pronto);

        if (buffer2[0] == EOF) {
            break;
        }

        printf("%s", buffer2);  // Imprime o conteúdo de buffer2
        sem_post(&sem_buffer2_vazio);
    }

    pthread_exit(NULL);
}
