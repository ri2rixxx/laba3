// Тест Миллера — программа для генерации и проверки простых чисел

#include <iostream>     // Для ввода-вывода
#include <vector>       // Для использования векторов
#include <algorithm>    // Для функций типа find
#include <random>       // Для генерации случайных чисел
#include <cmath>        // Для математических функций
#include <ctime>        // Для работы со временем
#include <locale.h>     // Для локализации
#include <numeric>      // Для числовых алгоритмов

using namespace std;

vector<int> primes;     // Глобальный вектор для хранения простых чисел

// Функция для поиска всех простых чисел до 500 с помощью решета Эратосфена
void sieveOfEratosthenes() {
    const int n = 500;                          // Верхняя граница поиска простых чисел
    vector<bool> is_prime(n + 1, true);        // Булев вектор, где true означает, что число возможно простое
    is_prime[0] = is_prime[1] = false;         // 0 и 1 не являются простыми числами

    // Перебираем числа от 2 до sqrt(n) и отсекаем их кратные
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;          // Кратные числа не простые
            }
        }
    }

    primes.clear();                             // Очищаем вектор primes перед заполнением
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);                // Добавляем простые числа в вектор
        }
    }
}

// Функция быстрого возведения в степень по модулю (a^b mod m)
long long mod_pow(long long a, long long b, long long m) {
    long long result = 1;                       // Инициализируем результат единицей
    a = a % m;                                 // Берём a по модулю m для сокращения чисел
    while (b > 0) {                            // Пока степень больше 0
        if (b % 2 == 1) {                      // Если текущий бит степени равен 1
            result = (result * a) % m;         // Умножаем результат на a по модулю
        }
        a = (a * a) % m;                       // Возводим a в квадрат по модулю
        b = b / 2;                             // Делим степень на 2 (сдвигаем биты вправо)
    }
    return result;                             // Возвращаем конечный результат
}

// Простейший тест Рабина-Миллера для проверки вероятной простоты числа
bool millerRabinTest(long long n, int k = 5) {
    if (n <= 1) return false;                   // Числа <= 1 не простые
    if (n == 2 || n == 3) return true;         // 2 и 3 — простые числа
    if (n % 2 == 0) return false;               // Чётные числа, кроме 2, не простые

    long long d = n - 1;
    int s = 0;
    while (d % 2 == 0) {                        // Разлагаем n-1 на 2^s * d
        d /= 2;
        s++;
    }

    random_device rd;                          // Источник случайности
    mt19937 gen(rd());                         // Генератор случайных чисел
    uniform_int_distribution<long long> dis(2, n - 2); // Диапазон для случайных оснований

    for (int i = 0; i < k; i++) {              // Повторяем тест k раз
        long long a = dis(gen);                 // Случайное основание a
        long long x = mod_pow(a, d, n);        // Вычисляем a^d mod n

        if (x == 1 || x == n - 1) continue;    // Если x равен 1 или n-1, переходим к следующей итерации

        for (int j = 0; j < s - 1; j++) {      // Иначе возводим x в квадрат s-1 раз
            x = mod_pow(x, 2, n);
            if (x == n - 1) break;              // Если x становится n-1, прерываем цикл
        }

        if (x != n - 1) return false;           // Если условие не выполнено, число составное
    }
    return true;                               // Если все проверки пройдены, число вероятно простое
}

// Функция для разложения числа n-1 на простые множители
vector<long long> factorize(long long n) {
    vector<long long> factors;                  // Вектор для хранения множителей
    long long d = n - 1;
    while (d % 2 == 0) {                        // Выделяем все множители 2
        factors.push_back(2);
        d /= 2;
    }

    // Для каждого простого числа из primes пытаемся делить d
    for (int p : primes) {
        if (p * p > d) break;                   // Если p^2 больше d, деление не нужно
        if (d % p == 0) {                       // Если p делит d
            factors.push_back(p);               // Добавляем p в список множителей
            while (d % p == 0) {                // Убираем все деления на p
                d /= p;
            }
        }
    }
    if (d > 1) {                                // Если остался множитель > 1, добавляем его
        factors.push_back(d);
    }
    return factors;                             // Возвращаем вектор простых множителей
}

// Расширенный тест Миллера (с использованием множителей n-1)
bool millerTest(long long n, int t = 5) {
    if (n <= 1) return false;                    // Не простые числа
    if (n == 2 || n == 3) return true;          // 2 и 3 простые
    if (n % 2 == 0) return false;                // Чётные числа — составные

    vector<long long> factors = factorize(n - 1); // Разложение n-1 на простые множители

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dis(2, n - 2);

    for (long long q : factors) {                // Для каждого множителя q
        bool all_conditions = false;             // Флаг, проверяющий условие
        for (int i = 0; i < t; ++i) {            // Проверяем t раз для разных оснований a
            long long a = dis(gen);               // Случайное основание a
            if (mod_pow(a, n - 1, n) != 1) {     // Проверка теоремы Ферма: a^(n-1) mod n == 1
                return false;                     // Если не выполняется, n составное
            }
            if (mod_pow(a, (n - 1) / q, n) != 1) { // Если a^((n-1)/q) mod n != 1
                all_conditions = true;            // Условие выполнено, можем перейти дальше
                break;
            }
        }
        if (!all_conditions) {                    // Если условие не выполнено ни для одного a
            return false;                          // n составное
        }
    }
    return true;                                  // Если все проверки прошли — вероятно простое
}

// Функция для генерации числа m — произведения случайных простых из primes
int generateM(int k) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> dis(0, primes.size() - 1);

    while (true) {                               // Бесконечный цикл, пока не найдём подходящее m
        int m = 1;
        for (int i = 0; i < k; ++i) {            // Умножаем k случайных простых чисел
            size_t index = dis(gen);
            m *= primes[index];

            if (m >= 49999) {                    // Если m стал слишком большим, прерываем и ищем заново
                m = -1;
                break;
            }
        }
        if (m != -1) {                           // Если нашли подходящее m
            return m;                            // Возвращаем его
        }
    }
}

// Функция генерации простых чисел
vector<int> generateMillerPrimes(int count) {
    vector<int> result;                          // Вектор для хранения найденных простых чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> k_dis(1, 3);  // Случайное число k от 1 до 3

    while (result.size() < count) {              // Пока не получим нужное количество чисел
        int k = k_dis(gen);                      // Случайное число k
        int m = generateM(k);                    // Генерируем m — произведение простых
        int n = 2 * m + 1;                       // Формируем кандидата для теста Миллера

	        if (n > 1 && n < 100000 && millerTest(n)) {  // Если n в диапазоне и прошло тест Миллера
            if (find(result.begin(), result.end(), n) == result.end()) { // Проверяем, что n нет в списке
                result.push_back(n);            // Добавляем в результат
            }
        }
        if (millerRabinTest(n)) {                // Также проверяем n тестом Миллера-Рабина
            if (find(result.begin(), result.end(), n) == result.end()) {
                result.push_back(n);
            }
        }
    }
    return result;                              // Возвращаем список сгенерированных простых чисел
}

// Функция для печати таблицы результатов тестов
void printResultsTable(const vector<int>& numbers, const string& test_name) {
    cout << "Результат теста " << test_name << " M-R" << ":\n";  // Заголовок таблицы
    cout << "| № | Число | Результат Миллера | Миллера-Рабина | \n";
    cout << "-----------------------------------------------------\n";

    int rejected = 0;                            // Счётчик отвергнутых чисел

    for (size_t i = 0; i < numbers.size(); ++i) {
        bool result = millerTest(numbers[i]);         // Результат теста Миллера
        bool miller_result = millerRabinTest(numbers[i]); // Результат теста Миллера-Рабина
        if (!result) rejected++;                       // Если не прошло тест Миллера, увеличиваем счётчик

        // Вывод строки таблицы с результатами
        cout << "| " << i + 1 << " | " << numbers[i] << " | "
             << (result ? "+" : "-") << " | "
             << (miller_result ? "+" : "-") << " |\n";
    }

    cout << "-----------------------------------------------------\n";
    cout << "k = " << rejected << "\n";               // Вывод общего количества отвергнутых чисел
}

// Главная функция — точка входа в программу
int main() {
    sieveOfEratosthenes();                     // Вычисляем простые числа до 500

    vector<int> miller_primes = generateMillerPrimes(10);  // Генерируем 10 чисел, прошедших тест Миллера
    printResultsTable(miller_primes, "Миллера");           // Печатаем таблицу результатов

    return 0;                                  // Завершаем программу успешно
}
