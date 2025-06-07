package main

import (
 "fmt"
 "math"
)

// Определяет, будет ли ряд иметь конечную сумму
func hasFiniteSum(base int) bool {
 return base > 1
}

// Вычисляет приближённую сумму ряда и возвращает как несократимую дробь
func approximateAsFraction(exponent, base int) (int, int) {
 const precision = 1e-10   // Порог значимости члена
 const scaleFactor = 1000000 // Масштаб для преобразования в целые числа

 var total float64
 for n := 1; ; n++ {
  term := math.Pow(float64(n), float64(exponent)) / math.Pow(float64(base), float64(n))
  if term < precision {
   break
  }
  total += term
 }

 // Преобразуем сумму в несократимую дробь
 numerator := int(math.Round(total * scaleFactor))
 denominator := scaleFactor
 common := gcd(numerator, denominator)

 return numerator / common, denominator / common
}

// Вычисляет наибольший общий делитель (НОД)
func gcd(a, b int) int {
 if b == 0 {
  return a
 }
 return gcd(b, a%b)
}

func main() {
 var a, b int

 fmt.Print("Введите значение степени a: ")
 fmt.Scan(&a)

 fmt.Print("Введите значение основания b: ")
 fmt.Scan(&b)

 // Если основание не позволяет убывать членам — сумма бесконечна
 if b == 1 || !hasFiniteSum(b) {
  fmt.Println("infinity")
  return
 }

 // Получаем приближенную рациональную сумму
 numerator, denominator := approximateAsFraction(a, b)
 approx := float64(numerator) / float64(denominator)

 // Сравниваем с приближённой суммой первых 100 членов
 var reference float64
 for n := 1; n < 100; n++ {
  reference += math.Pow(float64(n), float64(a)) / math.Pow(float64(b), float64(n))
 }

 // Если разница велика — считаем сумму иррациональной
 if math.Abs(approx-reference) > 1e-6 {
  fmt.Println("irrational")
 } else {
  fmt.Printf("%d/%d\n", numerator, denominator)
 }
}
