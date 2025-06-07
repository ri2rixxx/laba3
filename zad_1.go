package main

import (
    "fmt"
    "math"
)

func calculateY(x float64) float64 {
    switch {
    case x >= -7.0 && x <= -6.0:
        return 2.0
    case x > -6.0 && x <= -2.0:
        return -0.5*x - 1.0
    case x > -2.0 && x <= 0.0:
        sq := 4.0 - math.Pow(x+2.0, 2)
        if sq >= 0 {
            return 2.0 - math.Sqrt(sq)
        }
    case x > 0.0 && x <= 2.0:
        sq := 4.0 - x*x
        if sq >= 0 {
            return math.Sqrt(sq)
        }
    case x > 2.0 && x <= 3.0:
        return -x + 2.0
    }
    return 0.0
}

func main() {
    xStart := -7.0
    xEnd := 3.0
    dx := 0.5

    fmt.Println("Таблица значений функции:")
    fmt.Println("--------------------------")
    fmt.Printf("|  %5s |  %7s |\n", "x", "y(x)")
    fmt.Println("--------------------------")

    for x := xStart; x <= xEnd+0.001; x += dx {
        y := calculateY(x)
        fmt.Printf("| %5.1f | %7.2f |\n", x, y)
    }

    fmt.Println("--------------------------")
}
