package main

import (
 "fmt"
 "math"
 "math/rand"
 "time"
)

// primes хранит простые числа, сгенерированные решетом
var primes []int

// bitsize возвращает количество бит в числе n (без ведущих нулей)
func bitsize(n int64) int {
 if n == 0 {
  return 0
 }
 return 64 - bitsLeadingZeros64(uint64(n))
}

// bitsLeadingZeros64 считает количество ведущих нулей в 64-битном числе
func bitsLeadingZeros64(x uint64) int {
 // Используем встроенную функцию Go (есть в math/bits пакете, но реализуем вручную для совместимости)
 var n int
 for i := 63; i >= 0; i-- {
  if (x & (1 << i)) != 0 {
   break
  }
  n++
 }
 return n
}

// sieveOfEratosthenes вычисляет простые числа до 500
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

 primes = []int{}
 for i := 2; i <= n; i++ {
  if isPrime[i] {
   primes = append(primes, i)
  }
 }
}

// modPow вычисляет (a^b) mod m эффективно через бинарное возведение в степень
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

// millerRabinTest — классический тест Миллера-Рабина с k итерациями
func millerRabinTest(n int64, k int) bool {
 if n <= 1 {
  return false
 }
 if n == 2 || n == 3 {
  return true
 }
 if n%2 == 0 {
  return false
 }

 // представим n-1 как d*2^s
 d := n - 1
 s := 0
 for d%2 == 0 {
  d /= 2
  s++
 }

 for i := 0; i < k; i++ {
  a := rand.Int63n(n-3) + 2 // случайное a от 2 до n-2
  x := modPow(a, d, n)

  if x == 1 || x == n-1 {
   continue
  }

  contFlag := false
  for r := 0; r < s-1; r++ {
   x = modPow(x, 2, n)
   if x == n-1 {
    contFlag = true
    break
   }
  }

  if contFlag {
   continue
  }
  return false
 }

 return true
}

// pocklingtonTest — тест простоты по критерию Поклингтона
func pocklingtonTest(n int64, primes []int) bool {
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

 // факторизуем n-1 используя primes
 var factors []int64
 for _, p := range primes {
  if int64(p)*int64(p) > temp {
   break
  }
  if temp%int64(p) == 0 {
   factors = append(factors, int64(p))
   for temp%int64(p) == 0 {
    temp /= int64(p)
   }
  }
 }
 if temp > 1 {
  factors = append(factors, temp)
 }

 // произведение всех факторов
 F := int64(1)
 for _, q := range factors {
  F *= q
 }

 // проверка условия Ф > sqrt(n)
 if F <= int64(math.Sqrt(float64(n))) {
  return false
 }

 // проверка критериев Поклингтона
 for i := 0; i < 10; i++ {
  a := rand.Int63n(n-3) + 2
  if modPow(a, nMinus1, n) != 1 {
   return false
  }

  allConditions := true
  for _, q := range factors {
   if modPow(a, nMinus1/q, n) == 1 {
    allConditions = false
    break
   }
  }

  if allConditions {
   return true
  }
 }

 return false
}

// generatePocklingtonPrimes генерирует count простых чисел с помощью теста Поклингтона
func generatePocklingtonPrimes(count int) []int {
 result := []int{}
 targetPrimeBits := 16       // длина простого числа в битах
 halfBits := targetPrimeBits / 2
 FMinBits := halfBits + 1    // минимальный размер F в битах
 FMaxBits := halfBits + 2    // максимальный размер F в битах

 for len(result) < count {
  Ffactors := []int64{}
  F := int64(1)
  attempts := 0
  maxAttempts := 100

  // Подбор множителей F из простых чисел primes
  for bitsize(F) < FMinBits && attempts < maxAttempts {
   attempts++
   p := primes[rand.Intn(len(primes))]

   // Проверяем переполнение и дубли
   if bitsize(F*int64(p)) > FMaxBits {
    continue
   }

   alreadyExists := false
   for _, f := range Ffactors {
    if f == int64(p) {
     alreadyExists = true
     break
    }
   }
   if alreadyExists {
    continue
   }

   Ffactors = append(Ffactors, int64(p))
   F *= int64(p)
  }

  Fbits := bitsize(F)
  if Fbits < FMinBits || Fbits > FMaxBits {
   continue
  }
  
  Rbits := targetPrimeBits - Fbits
  if Rbits < 1 {
   continue
  }

  Rmin := int64(1) << (Rbits - 1)
  Rmax := (int64(1) << Rbits) - 1

  // делаем R четным
  if Rmin%2 != 0 {
   Rmin++
  }
  if Rmax%2 != 0 {
   Rmax--
  }
  if Rmax <= Rmin {
   continue
  }

  R := rand.Int63n(Rmax-Rmin+1) + Rmin
  if R%2 != 0 {
   R--
  }

  if R > (math.MaxInt64-1)/F {
   continue
  }

  n := R*F + 1

  if bitsize(n) != targetPrimeBits {
   continue
  }

  if pocklingtonTest(n, primes) {
   alreadyExists := false
   for _, val := range result {
    if val == int(n) {
     alreadyExists = true
     break
    }
   }
   if !alreadyExists {
    result = append(result, int(n))
   }
  }
 }

 return result
}

// printResultsTable выводит таблицу результатов тестов
func printResultsTable(numbers []int) {
 fmt.Println("| № | Число | Поклингтон | Миллера-Рабина |")
 fmt.Println("-----------------------------------------------------")

 rejectedMiller := 0

 for i, num := range numbers {
  pocklingtonRes := pocklingtonTest(int64(num), primes)
  millerRes := millerRabinTest(int64(num), 5)

  if !millerRes {
   rejectedMiller++
  }

  pockStr := "-"
  if pocklingtonRes {
   pockStr = "+"
  }
  millerStr := "-"
  if millerRes {
   millerStr = "+"
  }

  fmt.Printf("| %d | %d | %s | %s |\n", i+1, num, pockStr, millerStr)
 }

 fmt.Println("-----------------------------------------------------")
 fmt.Printf("k = %d\n", rejectedMiller)
}

func main() {
 rand.Seed(time.Now().UnixNano())

 // Генерируем простые числа для базы
 sieveOfEratosthenes()

 // Генерируем 10 простых чисел с помощью теста Поклингтона
 pocklingtonPrimes := generatePocklingtonPrimes(10)

 // Выводим результаты тестов
 printResultsTable(pocklingtonPrimes)
}
