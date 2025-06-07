#include <iostream>
#include <cmath>
#include <numeric> // Для std::gcd (наибольший общий делитель)
using namespace std;

// Функция определяет, будет ли ряд иметь конечную сумму
bool hasFiniteSum(int base) {
    // Сумма будет конечной, если основание степени больше 1
    return base > 1;
}

// Функция приближённо вычисляет сумму ряда и возвращает её в виде несократимой дроби
pair<int, int> approximateAsFraction(int exponent, int base) {
    const double precision = 1e-10;       // Минимальный вклад элемента в сумму
    const int scale = 1000000;            // Масштаб для преобразования в целые числа

    double total = 0.0;
    for (int n = 1; ; ++n) {
        double currentTerm = pow(n, exponent) / pow(base, n); // Текущий член ряда
        if (currentTerm < precision) break;                   // Прекращаем, если мал
        total += currentTerm;                                 // Прибавляем к общей сумме
    }

    // Преобразуем сумму в дробь с помощью масштабирования
    int numerator = static_cast<int>(round(total * scale));
    int commonDivisor = gcd(numerator, scale); // Упрощаем дробь
    return {numerator / commonDivisor, scale / commonDivisor};
}

int main() {
    int a, b;
    cout << "Введите значение степени a: ";
    cin >> a;
    cout << "Введите значение основания b: ";
    cin >> b;

    // Если b равно 1 — каждый член ряда равен 1, 2, 3... → сумма бесконечна
    if (b == 1) {
        cout << "infinity" << endl;
        return 0;
    }

    // Если основание b не обеспечивает сходимости — тоже бесконечная сумма
    if (!hasFiniteSum(b)) {
        cout << "infinity" << endl;
        return 0;
    }

    // Получаем дробное приближение суммы
    auto [num, denom] = approximateAsFraction(a, b);

    // Сравниваем с "точной" (условно) суммой из первых 100 членов
    double approxValue = static_cast<double>(num) / denom;
    double referenceSum = 0.0;
    for (int n = 1; n < 100; ++n) {
        referenceSum += pow(n, a) / pow(b, n);
    }

    // Если приближение и "точная" сумма сильно различаются — считаем иррациональной
    if (fabs(approxValue - referenceSum) > 1e-6) {
        cout << "irrational" << endl;
    } else {
        cout << num << "/" << denom << endl;
    }

    return 0;
}
