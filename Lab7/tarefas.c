#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaração das variáveis globais
char buffer1[N];
char buffer2[BUFFER_SIZE];
int leitura_concluida = 0;
int count1 = 0, count2 = 0;

// Declaração dos semáforos
sem_t sem_buffer1_vazio, sem_buffer2_vazio;
sem_t sem_buffer1_pronto, sem_buffer2_pronto;

// Função da thread 1
void *leitura(void *arg) {
    FILE *file = fopen("entrada.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&sem_buffer1_vazio);  // Aguarda espaço no buffer1

        if (fgets(buffer1, N, file) == NULL) { // Lê uma linha do arquivo
            buffer1[0] = EOF;  // Marca o fim do arquivo
            sem_post(&sem_buffer1_pronto);
            break;
        }

        sem_post(&sem_buffer1_pronto);  // Sinaliza que o buffer1 está pronto para processamento
    }

    fclose(file);
    pthread_exit(NULL);
}

// Função da thread 2
void *processamento(void *arg) {
    int input_index, output_index;
    int buffer1_size;
    int bloco_contador = 0;
    int char_contador;

    while (1) {
        sem_wait(&sem_buffer1_pronto);  // Aguarda dados no buffer1
        sem_wait(&sem_buffer2_vazio);   // Aguarda até que o buffer2 esteja vazio

        if (buffer1[0] == EOF) {  // Verifica o fim do arquivo
            buffer2[0] = EOF;  // Marca o fim no buffer2
            sem_post(&sem_buffer2_pronto);
            break;
        }

        buffer1_size = strlen(buffer1);
        input_index = 0;
        output_index = 0;
        char_contador = 0;

        // Copia blocos de dados do buffer1 para buffer2
        while (input_index < buffer1_size) {
            int bloco_tamanho = (bloco_contador <= 10) ? (2 * bloco_contador + 1) : 10;

            // Copia caracteres do bloco atual
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
        sem_post(&sem_buffer1_vazio);  // Sinaliza que buffer1 está vazio
        sem_post(&sem_buffer2_pronto); // Sinaliza que buffer2 está pronto para impressão
    }

    pthread_exit(NULL);
}

// Função da thread 3
void *impressao(void *arg) {
    while (1) {
        sem_wait(&sem_buffer2_pronto);  // Aguarda dados no buffer2

        if (buffer2[0] == EOF) {  // Verifica o fim do buffer
            break;
        }

        printf("%s", buffer2);
        count2 = 0;
        sem_post(&sem_buffer2_vazio);  // Sinaliza que buffer2 está vazio novamente
    }

    pthread_exit(NULL);
}
