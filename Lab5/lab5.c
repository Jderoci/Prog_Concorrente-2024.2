#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0;     // variável compartilhada entre as threads
pthread_mutex_t mutex; // variável de lock para exclusão mútua
pthread_cond_t cond;   // variável de condição para sincronização
int flag = 0;          // flag para controlar a execuçãao

// Função executada pela thread principal 
void *ExecutaTarefa(void *arg) {
  printf("Thread ExecutaTarefa está executando...\n");

  for (int i = 0; i < 100000; i++) {
    // Entrada na seção crítica
    pthread_mutex_lock(&mutex);
    soma++; 

    // Verificando se 'soma' é múltiplo de 10
    if (soma % 10 == 0) { 
      flag = 1; // Indica que um multiplo de 10 foi encontrado
      pthread_cond_signal(&cond);       // Sinaliza para a thread extra
      pthread_cond_wait(&cond, &mutex); // Espera a thread extra liberar
    }

    // Saída da seção crítica
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread ExecutaTarefa terminou!\n");
  pthread_exit(NULL);
}

// Função executada pela thread Extra
void *extra(void *arg) {
  int count = 0;
  printf("Thread Extra esta executando...\n");

  // Imprimindo os 20 primeiros valores de 'soma' que são múltiplos de 10
  while (count < 20) { 
    pthread_mutex_lock(&mutex);
    // Esperando um múltiplo de 10 ser encontrado
    while (flag == 0) { 
      pthread_cond_wait(&cond, &mutex);
    }

    // Printando o valor de 'soma'
    printf("soma = %ld\n", soma);
    count++;

    flag = 0;                   // Reseta a flag
    pthread_cond_signal(&cond); // Sinaliza para a thread principal continuar
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread Extra terminou!\n");
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t tid[2]; // identificadores das threads no sistema

  // Inicilaizando o mutex e a condição
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  // Criando a thread ExecutaTarefa
  if (pthread_create(&tid[0], NULL, ExecutaTarefa, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  // Criando a thread Extra
  if (pthread_create(&tid[1], NULL, extra, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  // Esperando as threads terminarem
  for (int t = 0; t < 2; t++) {
    if (pthread_join(tid[t], NULL)) {
      printf("--ERRO: pthread_join()\n");
      exit(-1);
    }
  }

  // Finalizando o mutex e a condição
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  printf("Valor final de 'soma' = %ld\n", soma);

  return 0;
}

