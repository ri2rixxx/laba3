package main

import (
 "fmt"
)

// Вспомогательная функция: возвращает максимум из двух чисел
func max(a, b int64) int64 {
 if a > b {
  return a
 }
 return b
}

func main() {
 var n, m int
 fmt.Print("Введите n и m: ")
 fmt.Scan(&n, &m) // Считываем количество чисел и максимальное количество, которое можно взять за ход

 nums := make([]int, n)
 fmt.Println("Ввод:")
 for i := range nums {
  fmt.Scan(&nums[i]) // Вводим числа последовательности
 }

 // cumulativeSum[i] — сумма всех чисел от i до конца
 cumulativeSum := make([]int64, n+1)
 for i := n - 1; i >= 0; i-- {
  cumulativeSum[i] = cumulativeSum[i+1] + int64(nums[i])
 }

 // dp[i] — наилучшая разница очков, которую можно получить, начиная с позиции i
 dp := make([]int64, n+1)

 // Проходим от конца массива к началу
 for i := n - 1; i >= 0; i-- {
  best := int64(-1 << 60) // Минимально возможное значение (имитация LLONG_MIN)
  for j := 1; j <= m && i+j <= n; j++ {
   // Сумма чисел, которые берёт игрок: cumulativeSum[i] - cumulativeSum[i+j]
   take := cumulativeSum[i] - cumulativeSum[i+j]

   // Вычисляем разницу очков (взято - то, что получит противник)
   diff := take - dp[i+j]

   // Сохраняем максимальную возможную разницу
   best = max(best, diff)
  }
  dp[i] = best // Оптимальный результат с позиции i
 }

 // Если dp[0] > 0, Павел выигрывает, иначе — Вика
 if dp[0] > 0 {
  fmt.Println("Результат: 1")
 } else {
  fmt.Println("Результат: 0")
 }
}
