#include <iostream>     // Стандартный ввод/вывод
#include <iomanip>      // Для управления форматированием чисел
#include <cmath>        // Для математических функций (exp, sqrt)
#include <vector>       // Динамические массивы

using namespace std;

// Модель Ньютона: вычисляет температуру в момент времени
double modelTemperature(double T0, double Ts, double r, double t) {
    return Ts + (T0 - Ts) * exp(-r * t);
}

// Линейная аппроксимация методом наименьших квадратов
void linearFit(const vector<double>& x, const vector<double>& y, double& slope, double& intercept) {
    int n = x.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; ++i) {
        sumX  += x[i];
        sumY  += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    slope     = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    intercept = (sumY - slope * sumX) / n;
}

// Расчёт коэффициента корреляции между двумя наборами данных
double correlationCoefficient(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    if (n == 0) return 0;

    double sumX = 0, sumY = 0;
    for (int i = 0; i < n; ++i) {
        sumX += x[i];
        sumY += y[i];
    }

    double avgX = sumX / n;
    double avgY = sumY / n;

    double numerator = 0, dx2 = 0, dy2 = 0;
    for (int i = 0; i < n; ++i) {
        double dx = x[i] - avgX;
        double dy = y[i] - avgY;
        numerator += dx * dy;
        dx2 += dx * dx;
        dy2 += dy * dy;
    }

    if (dx2 == 0 || dy2 == 0) return 0;
    return numerator / sqrt(dx2 * dy2);
}

// Безопасный ввод положительного числа
double readPositive(const string& label) {
    double value;
    while (true) {
        cout << label;
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка ввода. Повторите.\n";
        } else {
            return value;
        }
    }
}

int main() {
    // Переменные и массивы
    vector<double> timePoints, tempPoints;
    double T0, Ts, r, totalTime, dt;

    // Ввод исходных данных
    cout << "Начальная температура кофе (°C): ";
    cin >> T0;

    cout << "Температура окружающей среды (°C): ";
    cin >> Ts;

    r = readPositive("Коэффициент охлаждения (1/мин): ");
    totalTime = readPositive("Длительность моделирования (мин): ");
    dt = readPositive("Шаг времени (мин): ");

    // Заголовок таблицы
    cout << "\nМоделирование температуры кофе по закону Ньютона\n";
    cout << "----------------------------------------------\n";
    cout << setw(10) << "Время" << " | " << setw(15) << "Температура\n";
    cout << "----------------------------------------------\n";

    // Формат чисел
    cout << fixed << setprecision(2);

    // Расчёт температуры и сбор данных
    for (double t = 0; t <= totalTime; t += dt) {
        double T = modelTemperature(T0, Ts, r, t);
        timePoints.push_back(t);
        tempPoints.push_back(T);
        cout << setw(10) << t << " | " << setw(15) << T << "\n";
    }

    cout << "----------------------------------------------\n";

    // Аппроксимация прямой линии: T = a * t + b
    double a, b;
    linearFit(timePoints, tempPoints, a, b);

    // Расчёт корреляции
    double r_corr = correlationCoefficient(timePoints, tempPoints);

    // Вывод аппроксимирующей функции и корреляции
    cout << "\nЛинейная аппроксимация: T(t) = " << setprecision(4) << a << " * t + " << b << "\n";
    cout << "Коэффициент корреляции: r = " << r_corr << "\n";

    return 0;
}
