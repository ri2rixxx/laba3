package main

import (
 "fmt"
 "math"
)

// Вычисляет температуру в момент времени t по закону Ньютона
func modelTemperature(T0, Ts, r, t float64) float64 {
 return Ts + (T0-Ts)*math.Exp(-r*t)
}

// Метод наименьших квадратов для линейной аппроксимации: T(t) = a*t + b
func linearFit(x, y []float64) (a, b float64) {
 n := float64(len(x))
 var sumX, sumY, sumXY, sumX2 float64

 for i := range x {
  sumX += x[i]
  sumY += y[i]
  sumXY += x[i] * y[i]
  sumX2 += x[i] * x[i]
 }

 denominator := n*sumX2 - sumX*sumX
 if denominator == 0 {
  return 0, 0
 }

 a = (n*sumXY - sumX*sumY) / denominator
 b = (sumY - a*sumX) / n
 return
}

// Вычисляет коэффициент корреляции между x и y
func correlation(x, y []float64) float64 {
 n := len(x)
 if n == 0 {
  return 0
 }

 var sumX, sumY float64
 for i := 0; i < n; i++ {
  sumX += x[i]
  sumY += y[i]
 }
 meanX := sumX / float64(n)
 meanY := sumY / float64(n)

 var num, dx2, dy2 float64
 for i := 0; i < n; i++ {
  dx := x[i] - meanX
  dy := y[i] - meanY
  num += dx * dy
  dx2 += dx * dx
  dy2 += dy * dy
 }

 if dx2 == 0 || dy2 == 0 {
  return 0
 }
 return num / math.Sqrt(dx2*dy2)
}

// Безопасный ввод положительного числа
func readPositive(prompt string) float64 {
 var val float64
 for {
  fmt.Print(prompt)
  _, err := fmt.Scan(&val)
  if err == nil && val > 0 {
   return val
  }
  fmt.Println("Ошибка: введите положительное число.")
 }
}

func main() {
 var T0, Ts, r, totalTime, dt float64
 var timePoints, tempPoints []float64

 // Ввод начальных данных
 fmt.Print("Начальная температура кофе (°C): ")
 fmt.Scan(&T0)

 fmt.Print("Температура окружающей среды (°C): ")
 fmt.Scan(&Ts)

 r = readPositive("Коэффициент охлаждения (1/мин): ")
 totalTime = readPositive("Общее время моделирования (мин): ")
 dt = readPositive("Шаг по времени (мин): ")

 // Вывод шапки таблицы
 fmt.Println("\nМоделирование остывания кофе (метод Ньютона)")
 fmt.Println("----------------------------------------------")
 fmt.Printf("%10s | %15s\n", "Время", "Температура")
 fmt.Println("----------------------------------------------")

 // Расчёт температуры на каждом шаге
 for t := 0.0; t <= totalTime+1e-9; t += dt {
  T := modelTemperature(T0, Ts, r, t)
  timePoints = append(timePoints, t)
  tempPoints = append(tempPoints, T)
  fmt.Printf("%10.2f | %15.2f\n", t, T)
 }
 fmt.Println("----------------------------------------------")

 // Аппроксимация и корреляция
 a, b := linearFit(timePoints, tempPoints)
 rVal := correlation(timePoints, tempPoints)

 // Вывод уравнения и корреляции
 fmt.Printf("\nЛинейная аппроксимация: T(t) = %.4f * t + %.4f\n", a, b)
 fmt.Printf("Коэффициент корреляции: r = %.4f\n", rVal)
}
