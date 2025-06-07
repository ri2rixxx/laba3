package main

import (
 "fmt"
 "math/rand"
 "time"
)

// Глобальный срез для простых чисел
var primes []int

// Решето Эратосфена для поиска простых чисел до 500
func sieveOfEratosthenes() {
 n := 500
 isPrime := make([]bool, n+1)
 for i := range isPrime {
  isPrime[i] = true
 }
 isPrime[0], isPrime[1] = false, false // 0 и 1 не простые

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

// Быстрое возведение в степень по модулю: (a^b) % m
func modPow(a, b, m int64) int64 {
 var result int64 = 1
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

// Тест Миллера-Рабина (вероятностный тест на простоту)
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

 d := n - 1
 s := 0
 for d%2 == 0 {
  d /= 2
  s++
 }

 rnd := rand.New(rand.NewSource(time.Now().UnixNano()))
 for i := 0; i < k; i++ {
  a := rnd.Int63n(n-3) + 2 // случайное a в [2, n-2]
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
  if !contFlag {
   return false
  }
 }
 return true
}

// Разложение числа n-1 на простые множители (используя primes)
func factorize(n int64) []int64 {
 factors := []int64{}
 d := n - 1

 for d%2 == 0 {
  factors = append(factors, 2)
  d /= 2
 }

 for _, p := range primes {
  pi := int64(p)
  if pi*pi > d {
   break
  }
  if d%pi == 0 {
   factors = append(factors, pi)
   for d%pi == 0 {
    d /= pi
   }
  }
 }
 if d > 1 {
  factors = append(factors, d)
 }
 return factors
}

// Расширенный тест Миллера
func millerTest(n int64, t int) bool {
 if n <= 1 {
  return false
 }
 if n == 2 || n == 3 {
  return true
 }
 if n%2 == 0 {
  return false
 }

 factors := factorize(n)

 rnd := rand.New(rand.NewSource(time.Now().UnixNano()))
 for _, q := range factors {
  allConditions := false
  for i := 0; i < t; i++ {
   a := rnd.Int63n(n-3) + 2
   if modPow(a, n-1, n) != 1 {
    return false
   }
   if modPow(a, (n-1)/q, n) != 1 {
    allConditions = true
    break
   }
  }
  if !allConditions {
   return false
  }
 }
 return true
}

// Генерация числа m — произведения k случайных простых чисел
func generateM(k int) int {
 rnd := rand.New(rand.NewSource(time.Now().UnixNano()))
 for {
  m := 1
  for i := 0; i < k; i++ {
   index := rnd.Intn(len(primes))
   m *= primes[index]
   if m >= 49999 {
    m = -1
    break
   }
  }
  if m != -1 {
   return m
  }
 }
}

// Генерация чисел, прошедших тест Миллера
func generateMillerPrimes(count int) []int {
 result := []int{}
 rnd := rand.New(rand.NewSource(time.Now().UnixNano()))
 for len(result) < count {
  k := rnd.Intn(3) + 1             // k от 1 до 3
  m := generateM(k)                // генерируем m
  n := 2*m + 1                    // кандидат для проверки

  if n > 1 && n < 100000 && millerTest(int64(n), 5) {
   if !contains(result, n) {
    result = append(result, n)
   }
  }
  if millerRabinTest(int64(n), 5) {
   if !contains(result, n) {
    result = append(result, n)
   }
  }
 }
 return result
}

// Проверка наличия числа в срезе
func contains(slice []int, val int) bool {
 for _, v := range slice {
  if v == val {
   return true
  }
 }
 return false
}

// Печать таблицы результатов
func printResultsTable(numbers []int, testName string) {
 fmt.Printf("Результат теста %s M-R:\n", testName)
 fmt.Println("| № | Число | Результат Миллера | Миллера-Рабина |")
 fmt.Println("-----------------------------------------------------")

 rejected := 0
 for i, num := range numbers {
  result := millerTest(int64(num), 5)
  millerResult := millerRabinTest(int64(num), 5)
  if !result {
   rejected++
  }
  fmt.Printf("| %d | %d | %s | %s |\n", i+1, num, boolToSign(result), boolToSign(millerResult))
 }
 fmt.Println("-----------------------------------------------------")
 fmt.Printf("k = %d\n", rejected)
}

func boolToSign(b bool) string {
 if b {
  return "+"
 }
 return "-"
}

func main() {
 sieveOfEratosthenes()                   // Найти простые числа до 500

 millerPrimes := generateMillerPrimes(10) // Сгенерировать 10 чисел, прошедших тест Миллера
 printResultsTable(millerPrimes, "Миллера") // Вывести результаты в таблицу
}
