#ifndef TAREFAS_H
#define TAREFAS_H

// Funções das threads
void *leitura(void *arg);
void *processamento(void *arg);
void *impressao(void *arg);

#endif // TAREFAS_H
