#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long int soma = 0;     // variavel compartilhada entre as threads
pthread_mutex_t mutex; // variavel de lock para exclusao mutua
pthread_cond_t cond;   // variavel de condicao para sincronizacao
int flag = 0;          // flag para controlar a execucao

// funcao executada pela thread principal
void *ExecutaTarefa(void *arg) {
  printf("Thread de trabalho esta executando...\n");

  for (int i = 0; i < 100000; i++) {
    // --entrada na SC
    pthread_mutex_lock(&mutex);

    soma++; // incrementa a variavel compartilhada

    if (soma % 10 == 0) { // verifica se 'soma' e multiplo de 10
      flag = 1; // seta a flag para indicar que um multiplo de 10 foi encontrado
      pthread_cond_signal(&cond);       // sinaliza para a thread extra
      pthread_cond_wait(&cond, &mutex); // espera a thread extra liberar
    }

    // --saida da SC
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread de trabalho terminou!\n");
  pthread_exit(NULL);
}

// funcao executada pela thread extra
void *extra(void *arg) {
  int count = 0;
  printf("Thread extra esta executando...\n");

  while (
      count <
      20) { // imprime os 20 primeiros valores de 'soma' que sao multiplos de 10
    pthread_mutex_lock(&mutex);

    while (flag == 0) { // espera um multiplo de 10 ser encontrado
      pthread_cond_wait(&cond, &mutex);
    }

    // imprime o valor de 'soma'
    printf("soma = %ld\n", soma);
    count++;

    flag = 0;                   // reseta a flag
    pthread_cond_signal(&cond); // sinaliza para a thread principal continuar

    pthread_mutex_unlock(&mutex);
  }

  printf("Thread extra terminou!\n");
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t tid[2]; // identificadores das threads no sistema

  // --inicilaiza o mutex e a condicao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  //--cria a thread de trabalho
  if (pthread_create(&tid[0], NULL, ExecutaTarefa, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  //--cria a thread extra
  if (pthread_create(&tid[1], NULL, extra, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  //--espera as threads terminarem
  for (int t = 0; t < 2; t++) {
    if (pthread_join(tid[t], NULL)) {
      printf("--ERRO: pthread_join()\n");
      exit(-1);
    }
  }

  //--finaliza o mutex e a condicao
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  printf("Valor final de 'soma' = %ld\n", soma);

  return 0;
}

