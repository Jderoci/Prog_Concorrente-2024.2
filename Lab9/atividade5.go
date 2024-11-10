package main

import (
  "fmt"
  "math"
  "sync"
)

// Função para verificar a primalidade de um número
func ehPrimo(n int64) bool {
  if n <= 1 {
    return false
  }
  if n == 2 {
    return true
  }
  if n%2 == 0 {
    return false
  }
  limite := int64(math.Sqrt(float64(n))) + 1
  for i := int64(3); i < limite; i += 2 {
    if n%i == 0 {
      return false
    }
  }
  return true
}

func gerarNumeros(n int64, ch chan int64) {
  for i := int64(1); i <= n; i++ {
    ch <- i
  }
  close(ch)
}

func verificarPrimos(chEntrada chan int64, chSaida chan int64, wg *sync.WaitGroup) {
  defer wg.Done()
  for num := range chEntrada {
    if ehPrimo(num) {
      chSaida <- num
    }
  }
}

func main() {
  var N, M int64
  fmt.Print("Digite o valor de N: ")
  fmt.Scan(&N)
  fmt.Print("Digite o número de goroutines (M): ")
  fmt.Scan(&M)

  chEntrada := make(chan int64, 100) // Canal com buffer para evitar bloqueio
  chSaida := make(chan int64, 100)   // Canal com buffer para evitar bloqueio
  var wg sync.WaitGroup

  // Inicia goroutines para verificar números primos
  for i := int64(0); i < M; i++ {
    wg.Add(1)
    go verificarPrimos(chEntrada, chSaida, &wg)
  }

  // Gera os números a serem testados em uma goroutine separada
  go gerarNumeros(N, chEntrada)

  // Fecha o canal de saída após todas as goroutines terminarem
  go func() {
    wg.Wait()
    close(chSaida)
  }()

  // Conta e imprime os números primos encontrados
  totalPrimos := 0
  for primo := range chSaida {
    fmt.Printf("%d\n", primo)
    totalPrimos++
  }

  fmt.Printf("Total de números primos encontrados: %d\n", totalPrimos)
}

