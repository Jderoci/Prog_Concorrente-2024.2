// Canais na aplicação Go
package main

import (
	"fmt"
)

// Goroutine com diálogo
func tarefa(str chan string) {
	// Recebe mensagem de `main` e imprime
	msg := <-str
	fmt.Println(msg)

	// Envia resposta para `main`
	str <- "Oi Main, bom dia, tudo bem?"

	// Recebe segunda mensagem de `main` e imprime
	msg = <-str
	fmt.Println(msg)

	// Envia resposta final para `main`
	str <- "Certo, entendido."

	// Finaliza goroutine
	fmt.Println("finalizando goroutine")
	str <- "goroutine terminou"
}

func main() {
	// Canal para comunicação
	str := make(chan string)

	// Inicia a goroutine
	go tarefa(str)

	// Envia primeira mensagem
	str <- "Olá, Goroutine, bom dia!"

	// Recebe e imprime resposta
	fmt.Println(<-str)

	// Envia segunda mensagem
	str <- "Tudo bem! Vou terminar tá?"

	// Recebe e imprime resposta final
	fmt.Println(<-str)

	// Recebe confirmação de finalização da goroutine
	<-str
	fmt.Println("finalizando main")
}

