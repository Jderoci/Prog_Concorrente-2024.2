## Atividade 3 - Análise do Código

Este código implementa um sistema de controle de concorrência com prioridade para escritores utilizando `rwlock` da biblioteca `pthread`. 
A prioridade para operações de escrita é garantida pelo bloqueio dos leitores enquanto houver escritores pendentes ou ativos.

IMG DO LOG AQUI

### Funcionamento do Sistema
Podemos observar o comportamento do sistema no log gerado durante a execução com **16 threads** com ajustes na distribuição das operações de 80% para operações de leitura e 20% para escrita (divididas igualmente entre inserção e remoção). Abaixo estão alguns pontos que destacam a prioridade para escritores:

*1.* Quando um leitor tenta acessar o recurso enquanto há escritores ativos ou aguardando, ele entra em espera. 

*2.* Quando um escritor solicita acesso, ele é colocado na fila, mas uma vez que o recurso está disponível (ou todos os leitores terminam suas leituras), a prioridade é dada ao escritor, como mostrado no seguinte trecho:

*3.* Após a finalização de todos os escritores e com a fila de escritores vazia, os leitores são sinalizados para retomar a leitura.

### Conclusão
O log confirma que a implementação prioriza corretamente os escritores, bloqueando os leitores quando necessário. Dessa forma, o programa garante que operações de escrita, geralmente mais críticas, sejam realizadas com prioridade, mantendo a sincronização e eficiência no acesso aos recursos.


