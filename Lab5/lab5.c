#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0;     // variável compartilhada entre as threads
pthread_mutex_t mutex; // variável de lock para exclusão mútua
pthread_cond_t cond;   // variável de condição para sincronização
int cont = 0;          // contador de múltiplos de 10 impressos

// Função executada pela thread principal 
void *ExecutaTarefa(void *arg) {
  printf("Thread ExecutaTarefa está executando...\n");

  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&mutex);

    // Verificando se já imprimiram 20 múltiplos
    if (cont >= 20) {
      pthread_mutex_unlock(&mutex);
      break; // Sai do loop se já imprimiu 20 múltiplos
    }

    soma++; 

    // Verificando se 'soma' é múltiplo de 10 e se é um novo múltiplo
    if (soma % 10 == 0 && soma / 10 > cont) {
      pthread_cond_signal(&cond);       // Sinaliza a thread Extra
      pthread_cond_wait(&cond, &mutex); // Espera a thread Extra imprimir
    }

    pthread_mutex_unlock(&mutex);
  }

  printf("Thread ExecutaTarefa terminou!\n");
  pthread_exit(NULL);
}

// Função executada pela thread Extra
void *extra(void *arg) {
  printf("Thread Extra está executando...\n");

  while (cont < 20) {
    pthread_mutex_lock(&mutex);

    // Esperando por um múltiplo de 10 que ainda não tenha sido impresso
    while (soma % 10 != 0 || soma / 10 <= cont) {
      pthread_cond_wait(&cond, &mutex);
    }

    // Printando o valor de 'soma'
    printf("soma = %ld\n", soma);
    cont++;

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
