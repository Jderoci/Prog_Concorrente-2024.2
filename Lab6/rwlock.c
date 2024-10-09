#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

// lista compartilhada iniciada
struct list_node_s* head_p = NULL;

// quantidade de threads no programa
int nthreads;

// variáveis para controle de prioridade de escrita
pthread_rwlock_t rwlock;  // lock para controle de acesso
int esc_espera = 0;  // contagem de escritores esperando

// tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read;
   in = out = read = 0;

   // realiza operações de consulta (98%), inserção (1%) e remoção (1%)
   for(long int i = id; i < QTDE_OPS; i += nthreads) {
      op = rand() % 100;
      if (op < 98) {  // operação de leitura
         // verifica se há escritores esperando
         if (esc_espera > 0) {
            continue;  // se houver escritores, pula a operação de leitura para garantir prioridade
         }
         pthread_rwlock_rdlock(&rwlock);  // lock de leitura
         Member(i % MAX_VALUE, head_p);   // ignora valor de retorno
         pthread_rwlock_unlock(&rwlock);
         read++;
      } else if (op >= 98 && op < 99) {  // operação de inserção (escrita)
         __sync_add_and_fetch(&esc_espera, 1);  // incrementa o contador de escritores esperando
         pthread_rwlock_wrlock(&rwlock);  // lock de escrita
         Insert(i % MAX_VALUE, &head_p);  // ignora valor de retorno
         pthread_rwlock_unlock(&rwlock);
         __sync_sub_and_fetch(&esc_espera, 1);  // decrementa o contador de escritores esperando
         in++;
      } else if (op >= 99) {  // operação de remoção (escrita)
         __sync_add_and_fetch(&esc_espera, 1);  // incrementa o contador de escritores esperando
         pthread_rwlock_wrlock(&rwlock);  // lock de escrita
         Delete(i % MAX_VALUE, &head_p);  // ignora valor de retorno
         pthread_rwlock_unlock(&rwlock);
         __sync_sub_and_fetch(&esc_espera, 1);  // decrementa o contador de escritores esperando
         out++;
      }
   }

   // registra a quantidade de operações realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;

   // verifica se o número de threads foi passado na linha de comando
   if (argc < 2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   // insere os primeiros elementos na lista
   for (int i = 0; i < QTDE_INI; i++) {
      Insert(i % MAX_VALUE, &head_p);  // ignora valor de retorno
   }

   // aloca espaço de memória para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t) * nthreads);
   if (tid == NULL) {
      printf("--ERRO: malloc()\n");
      return 2;
   }

   // tomada de tempo inicial
   GET_TIME(ini);
   // inicializa a variável mutex
   pthread_rwlock_init(&rwlock, NULL);

   // cria as threads
   for (long int i = 0; i < nthreads; i++) {
      if (pthread_create(tid + i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n");
         return 3;
      }
   }

   // aguarda as threads terminarem
   for (int i = 0; i < nthreads; i++) {
      if (pthread_join(*(tid + i), NULL)) {
         printf("--ERRO: pthread_join()\n");
         return 4;
      }
   }

   // tomada de tempo final
   GET_TIME(fim);
   delta = fim - ini;
   printf("Tempo: %lf\n", delta);

   // libera o mutex
   pthread_rwlock_destroy(&rwlock);
   // libera o espaço de memória do vetor de threads
   free(tid);
   // libera o espaço de memória da lista
   Free_list(&head_p);

   return 0;
}

