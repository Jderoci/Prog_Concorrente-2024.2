#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para gerar valores aleatórios em vetores
void geraVetores(float *a, float *b, long int n) {
    for (long int i = 0; i < n; i++) {
        a[i] = (float)rand() / (float)(RAND_MAX / 10.0); // valores entre 0 e 10
        b[i] = (float)rand() / (float)(RAND_MAX / 10.0);
    }
}

// Função para calcular o produto interno de dois vetores
double calculaProdutoInterno(float *a, float *b, long int n) {
    double produto = 0.0;
    for (long int i = 0; i < n; i++) {
        produto += a[i] * b[i];
    }
    return produto;
}

// Função principal do programa sequencial
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Digite: %s <dimensão N> <nome do arquivo>\n", argv[0]);
        return 1;
    }

    long int n = atol(argv[1]);
    char *nome_arquivo = argv[2];
    float *a = (float *)malloc(n * sizeof(float));
    float *b = (float *)malloc(n * sizeof(float));
    if (a == NULL || b == NULL) {
        printf("Erro de alocação de memória\n");
        return 2;
    }

    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios
    geraVetores(a, b, n);

    double produto_interno = calculaProdutoInterno(a, b, n);

    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 3;
    }

    fwrite(&n, sizeof(long int), 1, arquivo);
    fwrite(a, sizeof(float), n, arquivo);
    fwrite(b, sizeof(float), n, arquivo);
    fwrite(&produto_interno, sizeof(double), 1, arquivo);

    fclose(arquivo);
    free(a);
    free(b);

    printf("Produto interno calculado e salvo no arquivo '%s'.\n", nome_arquivo);

    return 0;
}

