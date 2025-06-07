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

// Глобальный вектор простых чисел
vector<int> primes;

// Функция для определения количества значащих битов в числе n (64-битное)
int bitsize(long long n) {
    // Если n = 0, битов нет, иначе считаем ведущие нули и вычитаем из 64
    return n == 0 ? 0 : 64 - __builtin_clzll(n);
}

// Решето Эратосфена для поиска простых чисел до 500
void sieveOfEratosthenes() {
    const int n = 500;
    vector<bool> is_prime(n + 1, true); // Вектор пометок для чисел [0..n]
    is_prime[0] = is_prime[1] = false;  // 0 и 1 не простые

    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            // Отметить кратные p как составные
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    primes.clear();
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i); // Добавляем найденные простые в глобальный список
        }
    }
}

// Быстрое возведение в степень по модулю: (a^b) mod m
long long mod_pow(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b % 2 == 1) {  // Если текущий бит b установлен
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b = b / 2;  // Сдвиг вправо по битам (деление на 2)
    }
    return result;
}

// Тест Миллера-Рабина - вероятностный тест на простоту числа n
bool millerRabinTest(long long n, int k = 5) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false; // Чётные числа >2 составные

    long long d = n - 1;
    int s = 0;

    // Представляем n-1 как d * 2^s, где d нечётное
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dis(2, n - 2); // Диапазон выбора a

    for (int i = 0; i < k; i++) {
        long long a = dis(gen);
        long long x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1) continue; // Проверка первой части условия

        bool passed = false;
        for (int j = 0; j < s - 1; j++) {
            x = mod_pow(x, 2, n);
            if (x == n - 1) {
                passed = true;
                break;
            }
        }

        if (!passed) return false; // Если условие не выполняется - число составное
    }
    return true; // Вероятно простое
}

// Тест Поклингтона для проверки простоты числа n, используя набор простых делителей
bool pocklingtonTest(long long n, const vector<int>& primes) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    vector<long long> factors;
    long long n_minus_1 = n - 1;
    long long temp = n_minus_1;

    // Факторизация n-1 по простым из глобального списка primes
    for (int p : primes) {
        if (p * p > temp) break;
        if (temp % p == 0) {
            factors.push_back(p);
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        factors.push_back(temp); // Остаток — простой множитель
    }

    long long F = 1;
    for (long long q : factors) {
        F *= q; // Произведение всех факторов
    }

    // Если произведение факторов слишком мало (<= корня из n) — тест неприменим
    if (F <= sqrt(n)) {
        return false;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dis(2, n - 2);

    for (int i = 0; i < 10; ++i) {
        long long a = dis(gen);
        if (mod_pow(a, n - 1, n) != 1) {
            return false; // Условие Ферма не выполнено
        }

	bool all_conditions = true;
        for (long long q : factors) {
            // Проверка условия Поклингтона для всех делителей q
            if (mod_pow(a, (n - 1) / q, n) == 1) {
                all_conditions = false;
                break;
            }
        }

        if (all_conditions) {
            return true; // Число вероятно простое
        }
    }

    return false; // Число составное
}

// Генерация count простых чисел методом Поклингтона с размером примерно 16 бит
vector<int> generatePocklingtonPrimes(int count) {
    random_device rd;
    mt19937 gen(rd());

    vector<int> result;
    const int target_prime_bits = 16;          // Целевой размер простого в битах
    const int half_bits = target_prime_bits / 2;

    const int F_min_bits = half_bits + 1;      // Минимальный размер произведения факторов
    const int F_max_bits = half_bits + 2;      // Максимальный размер

    while (result.size() < count) {
        vector<int> F_factors;
        long long F = 1;
        int attempts = 0;
        const int max_attempts = 100;

        // Подбор множителей для F, чтобы F был подходящего размера
        while (bitsize(F) < F_min_bits && attempts++ < max_attempts) {
            uniform_int_distribution<size_t> prime_dis(0, primes.size() - 1);
            int p = primes[prime_dis(gen)];

            if (bitsize(F * p) > F_max_bits) continue; // Переполнение по битам
            if (find(F_factors.begin(), F_factors.end(), p) == F_factors.end()) {
                F_factors.push_back(p);
                F *= p;
            }
        }

        int F_bits = bitsize(F);
        if (F_bits < F_min_bits || F_bits > F_max_bits) continue; // Несоответствие размеру

        const int R_bits = target_prime_bits - F_bits; // Размер R, чтобы n = R * F + 1 был нужного размера
        if (R_bits < 1) continue;

        long long R_min = 1LL << (R_bits - 1);
        long long R_max = (1LL << R_bits) - 1;

        if (R_min % 2 != 0) R_min++; // R должен быть чётным
        if (R_max % 2 != 0) R_max--;

        if (R_max <= R_min) continue;

        uniform_int_distribution<long long> R_dis(R_min, R_max);
        long long R = R_dis(gen);
        if (R % 2 != 0) R--;

        if (R > (numeric_limits<long long>::max() - 1) / F) continue; // Проверка на переполнение

        long long n = R * F + 1;

        if (bitsize(n) != target_prime_bits) continue;

        if (pocklingtonTest(n, primes)) {
            if (find(result.begin(), result.end(), n) == result.end()) {
                result.push_back(static_cast<int>(n)); // Добавляем простое число
            }
        }
    }
    return result;
}

// Вывод таблицы результатов с тестами
void printResultsTable(const vector<int>& numbers) {
    cout << "| № | Число | Поклингтон | Миллера-Рабина |\n";
    cout << "-----------------------------------------------------\n";

    int rejected_miller = 0;

    for (size_t i = 0; i < numbers.size(); ++i) {
        bool pocklington_result = pocklingtonTest(numbers[i], primes);
        bool miller_result = millerRabinTest(numbers[i]);

        if (!miller_result) rejected_miller++;

        cout << "| " << i + 1 << " | " << numbers[i] << " | "
             << (pocklington_result ? "+" : "-") << " | "
             << (miller_result ? "+" : "-") << " |\n";
    }

    cout << "-----------------------------------------------------\n";
    cout << "k = " << rejected_miller << "\n";
}

int main() {
    sieveOfEratosthenes(); // Вычисляем простые числа до 500

    vector<int> pocklington_primes = generatePocklingtonPrimes(10); // Генерируем 10 простых чисел
    printResultsTable(pocklington_primes);                          // Выводим результаты

    return 0;
}
