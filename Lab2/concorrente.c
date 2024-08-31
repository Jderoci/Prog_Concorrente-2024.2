#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Estrutura para passar os argumentos para as threads
typedef struct {
    long int id;
    long int n;
    int nthreads;
    float *a;
    float *b;
    double *resultado_parcial;
} t_Args;

// Função executada pelas threads para calcular uma parte do produto interno
void *calculaProdutoInternoConcorrente(void *arg) {
    t_Args *args = (t_Args *) arg;
    double soma_local = 0.0;

    for (long int i = args->id; i < args->n; i += args->nthreads) {
        soma_local += args->a[i] * args->b[i];
    }

    *(args->resultado_parcial) = soma_local;
    pthread_exit(NULL);
}

// Função principal do programa concorrente
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Digite: %s <número de threads> <nome do arquivo>\n", argv[0]);
        return 1;
    }

    int nthreads = atoi(argv[1]);
    char *nome_arquivo = argv[2];
    long int n;
    float *a, *b;
    double produto_interno_esperado, produto_interno_calculado = 0.0;

    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 2;
    }

    fread(&n, sizeof(long int), 1, arquivo);
    a = (float *)malloc(n * sizeof(float));
    b = (float *)malloc(n * sizeof(float));
    if (a == NULL || b == NULL) {
        printf("Erro de alocação de memória\n");
        return 3;
    }
    fread(a, sizeof(float), n, arquivo);
    fread(b, sizeof(float), n, arquivo);
    fread(&produto_interno_esperado, sizeof(double), 1, arquivo);
    fclose(arquivo);

    pthread_t tid[nthreads];
    t_Args args[nthreads];
    double resultado_parcial[nthreads];

    // Criação das threads
    for (long int i = 0; i < nthreads; i++) {
        args[i].id = i;
        args[i].n = n;
        args[i].nthreads = nthreads;
        args[i].a = a;
        args[i].b = b;
        args[i].resultado_parcial = &resultado_parcial[i];
        if (pthread_create(&tid[i], NULL, calculaProdutoInternoConcorrente, (void *)&args[i])) {
            printf("ERRO: pthread_create()\n");
            return 4;
        }
    }

    // Aguarda o término das threads e soma os resultados parciais
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("ERRO: pthread_join()\n");
            return 5;
        }
        produto_interno_calculado += resultado_parcial[i];
    }

    // Cálculo do erro relativo
    double erro_relativo = fabs((produto_interno_esperado - produto_interno_calculado) / produto_interno_esperado);

    printf("Produto interno esperado = %.15lf\n", produto_interno_esperado);
    printf("Produto interno calculado = %.15lf\n", produto_interno_calculado);
    printf("Erro relativo = %.15lf\n", erro_relativo);

    free(a);
    free(b);

    return 0;
}
