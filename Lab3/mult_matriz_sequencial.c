#include<stdio.h>
#include<stdlib.h>
#include "timer.h"

float *mat1;
float *mat2;
float *sai;
int dim1, dim2, dim3;

// Função para carregar a matriz de um arquivo binário
void le_matriz_bin(char *arquivo, float **matriz, int *linhas, int *colunas) {
    FILE *f = fopen(arquivo, "rb");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    fread(linhas, sizeof(int), 1, f);
    fread(colunas, sizeof(int), 1, f);

    *matriz = (float*) malloc((*linhas) * (*colunas) * sizeof(float));
    if (!(*matriz)) {
        printf("Erro ao alocar memória\n");
        exit(2);
    }

    fread(*matriz, sizeof(float), (*linhas) * (*colunas), f);
    fclose(f);
}

// Função para salvar a matriz de saída em um arquivo binário
void salva_matriz_bin(char *arquivo, float *matriz, int linhas, int colunas) {
    FILE *f = fopen(arquivo, "wb");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    fwrite(&linhas, sizeof(int), 1, f);
    fwrite(&colunas, sizeof(int), 1, f);
    fwrite(matriz, sizeof(float), linhas * colunas, f);
    fclose(f);
}

// Função que realiza a multiplicação de matrizes de forma sequencial
void mult_matriz_sequencial() {
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim3; j++) {
            for (int k = 0; k < dim2; k++) {
                sai[i * dim3 + j] += mat1[i * dim2 + k] * mat2[k * dim3 + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    double inicio, fim, delta;

    GET_TIME(inicio);
    if (argc < 4) {
        puts("A entrada deve ser do tipo: ./sequencial <mat1> <mat2> <saida>");
        return 1;
    }

    // Leitura das matrizes de arquivos binários
    le_matriz_bin(argv[1], &mat1, &dim1, &dim2);
    le_matriz_bin(argv[2], &mat2, &dim2, &dim3);

    // Alocação da matriz de saída
    sai = (float*) malloc(dim1 * dim3 * sizeof(float));
    if (!sai) {
        printf("Erro -- malloc\n");
        return 2;
    }

    // Inicialização da matriz de saída
    for (int i = 0; i < dim1 * dim3; i++) {
        sai[i] = 0;
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicializacao (sequencial): %lf\n", delta);

    GET_TIME(inicio);
    
    // Multiplicação de matrizes de forma sequencial
    mult_matriz_sequencial();

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento (sequencial): %lf\n", delta);

    // Salvar a matriz de saída
    salva_matriz_bin(argv[3], sai, dim1, dim3);

    // Liberação de memória
    GET_TIME(inicio);
    free(mat1);
    free(mat2);
    free(sai);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalizacao (sequencial): %lf\n", delta);

    return 0
}
