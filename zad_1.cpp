#include <iostream>   // для ввода-вывода
#include <iomanip>    // для форматирования вывода
#include <cmath>      // для математических функций

using namespace std;

// Функция для вычисления значения y в зависимости от x
double calculateY(double x) {
    // Участок с постоянным значением: от -7 до -6 включительно
    if (x >= -7.0 && x <= -6.0) {
        return 2.0;
    }
    // Линейный участок: от -6 до -2 — от точки (-6, -1) до (-2, 0)
    else if (x > -6.0 && x <= -2.0) {
        return 0.25 * x + 0.5;
    }
    // Нижняя полуокружность: центр (-2, 2), радиус 2, от -2 до 0
    else if (x > -2.0 && x <= 0.0) {
        double sq = 4.0 - pow(x + 2.0, 2);
        return (sq >= 0) ? 2.0 - sqrt(sq) : 0.0;
    }
    // Верхняя полуокружность: центр (0, 0), радиус 2, от 0 до 2
    else if (x > 0.0 && x <= 2.0) {
        double sq = 4.0 - x * x;
        return (sq >= 0) ? sqrt(sq) : 0.0;
    }
    // Линейный участок: от 2 до 3 — от (2, 0) до (3, -1)
    else if (x > 2.0 && x <= 3.0) {
        return -x + 2.0;
    }

    // Значения вне диапазона
    return 0.0;
}

int main() {
    const double x_start = -7.0;
    const double x_end = 3.0;
    const double dx = 0.5;

    // Заголовок таблицы
    cout << "Таблица значений функции:\n";
    cout << "--------------------------\n";
    cout << "|   x   |   y(x)   |\n";
    cout << "--------------------------\n";

    // Вывод значений функции
    for (double x = x_start; x <= x_end + 0.001; x += dx) {
        double y = calculateY(x);
        cout << "| " << setw(5) << x << " | "
             << fixed << setprecision(2) << setw(8) << y << " |\n";
    }

    cout << "--------------------------\n";
    return 0;
}
