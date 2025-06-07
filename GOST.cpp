// Тест ГОСТ (алгоритм проверки простоты по ГОСТ)

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <ctime>
#include <locale.h>
#include <numeric>
#include <limits>
#include <bitset>

using namespace std;

const int NUMOFPRIMES = 10;         // Количество простых чисел, которые нужно сгенерировать
vector<int> primes;                 // Вектор для хранения простых чисел, полученных решетом

// Функция вычисления размера числа в битах
int binSize(long long n) {
    // __builtin_clzll - встроенная функция для подсчёта ведущих нулей в числе
    return n == 0 ? 0 : 64 - __builtin_clzll(n); // размер числа в битах
}

// Округление результата деления a/b вверх
int bigRound(int a, int b) {
    return (a + b - 1) / b;
}

// Шаблонная функция для выбора случайного элемента из вектора
template<typename T>
T randomChoice(const vector<T>& vec, mt19937_64& gen) {
    uniform_int_distribution<size_t> dis(0, vec.size() - 1);
    return vec[dis(gen)];
}

// Решето Эратосфена для генерации простых чисел до 500
void sieveOfEratosthenes() {
    const int n = 500;
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    primes.clear();
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}

// Быстрое возведение в степень по модулю: (a^b) % m
long long mod_pow(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b % 2 == 1) {        // Если текущий бит b равен 1
            result = (result * a) % m;
        }
        a = (a * a) % m;         // Квадрат основания
        b = b / 2;               // Сдвиг b вправо (деление на 2)
    }
    return result;
}

// Тест ГОСТ (проверка простоты числа n)
bool gostTest(long long n, const vector<int>& primes) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    long long n_minus_1 = n - 1;
    long long temp = n_minus_1;
    long long q = 1;

    // Факторизация n-1 на простой делитель q (из списка простых)
    for (int p : primes) {
        if (p * p > temp) break;
        if (temp % p == 0) {
            q = p;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        q = temp; // если осталось число >1, значит это простой делитель q
    }

    // Проверка по теореме Диемитко:
    if (mod_pow(2, n - 1, n) != 1) {
        return false;
    }

    if (mod_pow(2, (n - 1) / q, n) == 1) {
        return false;
    }

    return true;
}

// Генерация простых чисел по тесту ГОСТ
vector<int> generateGostPrimes(int t, const vector<int>& primes, mt19937_64& gen) {
    vector<int> result;
    uniform_real_distribution<double> dist(0.1, 1.0);

    const int pow2t1 = 1 << (t - 1); // 2^(t-1)
    const int pow2t = 1 << t;        // 2^t

    while (result.size() < NUMOFPRIMES) {
        int q = randomChoice(primes, gen);
        while (binSize(q) > bigRound(t, 2)) { // выбираем q по размеру
            q = randomChoice(primes, gen);
        }

        bool isFound = false;

        while (!isFound) {
            double ksi = dist(gen);
            int NN = bigRound(pow2t1, q) + bigRound(pow2t1 * ksi, q);
            int N = NN % 2 == 0 ? NN : NN + 1;  // делаем N четным
            int u = 0;

            while (!isFound) {
                int p = (N + u) * q + 1;
                if (p > pow2t) break;

                // Проверка условий ГОСТ теста
                if (mod_pow(2, p - 1, p) == 1 && mod_pow(2, N + u, p) != 1) {
                    result.push_back(p);
                    isFound = true;
                }
                u += 2; // увеличиваем u на 2 (чтобы сохранить p нечётным)
            }
        }
    }

    return result;
}

// Вывод таблицы результатов
void printResultsTable(const vector<int>& numbers) {
    cout << "| № | Число | ГОСТ |\n";
    cout << "------------------------------------\n";

    int rejected = 0;

    for (size_t i = 0; i < numbers.size(); ++i) {
        bool gost_result = gostTest(numbers[i], primes);

        if (!gost_result) rejected++;

        cout << "| " << i + 1 << " | " << numbers[i] << " | "
             << (gost_result ? "+" : "-") << " |\n";
    }

    cout << "k = " << rejected << "\n";
}

int main() {
    mt19937_64 gen(time(0));   // Инициализация генератора случайных чисел
    sieveOfEratosthenes();      // Генерация простых чисел для теста ГОСТ

    vector<int> gost_primes = generateGostPrimes(16, primes, gen); // Генерация простых чисел ГОСТ

    printResultsTable(gost_primes); // Вывод результатов

    return 0;
}
