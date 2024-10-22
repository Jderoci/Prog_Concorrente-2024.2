#include <pthread.h>
#include "sincronizacao.h"
#include "tarefas.h"

int main() {
    pthread_t threads[3];

    iniciar_sincronizacao();

    pthread_create(&threads[0], NULL, leitura, NULL);
    pthread_create(&threads[1], NULL, processamento, NULL);
    pthread_create(&threads[2], NULL, impressao, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    finalizar_sincronizacao();
    return 0;
}
