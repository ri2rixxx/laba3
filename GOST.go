package main

import (
 "fmt"
 "math/rand"
 "time"
)

const NUMOFPRIMES = 10

var primes []int

// Возвращает размер числа n в битах
func binSize(n int64) int {
 if n == 0 {
  return 0
 }
 return 64 - bitsLeadingZeros64(uint64(n))
}

// Аналог __builtin_clzll - количество ведущих нулей в числе
func bitsLeadingZeros64(x uint64) int {
 n := 0
 for i := 63; i >= 0; i-- {
  if (x & (1 << i)) != 0 {
   break
  }
  n++
 }
 return n
}

// Округление вверх результата a/b
func bigRound(a, b int) int {
 return (a + b - 1) / b
}

// Выбор случайного элемента из среза
func randomChoice(slice []int) int {
 return slice[rand.Intn(len(slice))]
}

// Решето Эратосфена для простых чисел до 500
func sieveOfEratosthenes() {
 n := 500
 isPrime := make([]bool, n+1)
 for i := range isPrime {
  isPrime[i] = true
 }
 isPrime[0], isPrime[1] = false, false

 for p := 2; p*p <= n; p++ {
  if isPrime[p] {
   for i := p * p; i <= n; i += p {
    isPrime[i] = false
   }
  }
 }

 primes = primes[:0]
 for i := 2; i <= n; i++ {
  if isPrime[i] {
   primes = append(primes, i)
  }
 }
}

// Быстрое возведение в степень по модулю
func modPow(a, b, m int64) int64 {
 result := int64(1)
 a = a % m
 for b > 0 {
  if b&1 == 1 {
   result = (result * a) % m
  }
  a = (a * a) % m
  b >>= 1
 }
 return result
}

// Тест ГОСТ проверки простоты числа n
func gostTest(n int64, primes []int) bool {
 if n <= 1 {
  return false
 }
 if n == 2 || n == 3 {
  return true
 }
 if n%2 == 0 {
  return false
 }

 nMinus1 := n - 1
 temp := nMinus1
 var q int64 = 1

 for _, p := range primes {
  if int64(p)*int64(p) > temp {
   break
  }
  if temp%int64(p) == 0 {
   q = int64(p)
   for temp%int64(p) == 0 {
    temp /= int64(p)
   }
  }
 }
 if temp > 1 {
  q = temp
 }

 if modPow(2, n-1, n) != 1 {
  return false
 }

 if modPow(2, (n-1)/q, n) == 1 {
  return false
 }

 return true
}

// Генерация простых чисел ГОСТ
func generateGostPrimes(t int, primes []int) []int {
 result := []int{}

 pow2t1 := 1 << (t - 1)
 pow2t := 1 << t

 for len(result) < NUMOFPRIMES {
  q := randomChoice(primes)
  for binSize(int64(q)) > bigRound(t, 2) {
   q = randomChoice(primes)
  }

  isFound := false

  for !isFound {
   ksi := rand.Float64()*(1.0-0.1) + 0.1
   NN := bigRound(pow2t1, q) + bigRound(int(float64(pow2t1)*ksi), q)
   N := NN
   if N%2 != 0 {
    N++
   }
   u := 0

   for !isFound {
    p := (N + u)*q + 1
    if p > pow2t {
     break
    }

    if modPow(2, int64(p-1), int64(p)) == 1 && modPow(2, int64(N+u), int64(p)) != 1 {
     result = append(result, p)
     isFound = true
    }
    u += 2
   }
  }
 }

 return result
}

// Вывод таблицы результатов
func printResultsTable(numbers []int) {
 fmt.Println("| № | Число | ГОСТ |")
 fmt.Println("------------------------------------")

 rejected := 0

 for i, num := range numbers {
  gostRes := gostTest(int64(num), primes)
  if !gostRes {
   rejected++
  }
  mark := "-"
  if gostRes {
   mark = "+"
  }
  fmt.Printf("| %d | %d | %s |\n", i+1, num, mark)
 }

 fmt.Printf("k = %d\n", rejected)
}

func main() {
 rand.Seed(time.Now().UnixNano())
 sieveOfEratosthenes()

 gostPrimes := generateGostPrimes(16, primes)
 printResultsTable(gostPrimes)
}
