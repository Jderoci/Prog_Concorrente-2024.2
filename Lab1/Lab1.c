#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int *vetor;
int N, M;

// Função executada por cada thread
void *soma_vetor(void *arg) {
    int id = *(int *)arg; // Identificador da thread
    int inicio = id * (N / M); // Calcula o início do intervalo para esta thread
    int fim = (id + 1) * (N / M); // Calcula o fim do intervalo para esta thread

    if (id == M - 1) { // Ajuste para a última thread
        fim = N; // A última thread processa até o final do vetor
    }

    for (int i = inicio; i < fim; i++) {
        vetor[i] += 1; // Incrementa cada elemento no intervalo
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <tamanho do vetor N> <numero de threads M>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);

    if (N <= 0 || M <= 0) {
        printf("N e M devem ser maiores que 0.\n");
        return 1;
    }

    vetor = (int *)malloc(N * sizeof(int));
    if (vetor == NULL) {
        printf("Erro ao alocar memória para o vetor.\n");
        return 1;
    }

    pthread_t threads[M];
    int ids[M];

    // Inicializa o vetor
    for (int i = 0; i < N; i++) {
        vetor[i] = i; // Inicializa com valores de 0 a N-1
    }

    // Cria as threads
    for (int i = 0; i < M; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, soma_vetor, (void *)&ids[i]) != 0) {
            printf("Erro ao criar a thread %d\n", i);
            free(vetor);
            return 1;
        }
    }

    // Espera as threads terminarem
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verifica e imprime o vetor final
    int correto = 1;
    for (int i = 0; i < N; i++) {
        if (vetor[i] != i + 1) {
            correto = 0;
            break;
        }
    }

    if (correto) {
        printf("Os valores do vetor estão corretos!\n");
    } else {
        printf("Os valores do vetor não estão corretos.\n");
    }

    // Imprime o vetor final
    /*
    printf("Vetor final: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
    */

    // Libera a memória alocada
    free(vetor);

    return 0;
}
