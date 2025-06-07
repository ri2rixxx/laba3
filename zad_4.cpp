#include <iostream>   // Для работы с вводом и выводом
#include <vector>     // Для использования динамических массивов (векторов)
#include <climits>    // Для константы LLONG_MIN

using namespace std;

int main() {
    int totalNumbers, maxTake; // totalNumbers — количество чисел, maxTake — максимум чисел за ход

    cout << "Введите n и m: ";
    cin >> totalNumbers >> maxTake; // Считываем значения n и m

    vector<int> sequence(totalNumbers); // Массив чисел, из которых выбирают игроки

    cout << "Ввод: ";
    for (int i = 0; i < totalNumbers; ++i) {
        cin >> sequence[i]; // Вводим каждый элемент массива
    }

    // Массив для хранения максимальной разницы очков от позиции i до конца
    vector<long long> optimalGain(totalNumbers + 1, 0);

    // cumulativeSum[i] — сумма всех чисел от позиции i до конца массива
    vector<long long> cumulativeSum(totalNumbers + 1, 0);

    // Заполняем cumulativeSum с конца 
    for (int i = totalNumbers - 1; i >= 0; --i) {
        cumulativeSum[i] = cumulativeSum[i + 1] + sequence[i];
    }

    // Обратный проход: вычисляем лучший возможный счёт для каждого начала позиции
    for (int pos = totalNumbers - 1; pos >= 0; --pos) {
        long long bestCurrentMove = LLONG_MIN; // Инициализируем минимумом — будем искать максимум

        // Пробуем взять от 1 до maxTake элементов, если они не выходят за границу
        for (int step = 1; step <= maxTake && pos + step <= totalNumbers; ++step) {
            // Вычисляем сумму элементов, которые берёт текущий игрок
            long long currentSum = cumulativeSum[pos] - cumulativeSum[pos + step];

            // Разница очков: то, что взял игрок, минус то, что получит соперник
            long long result = currentSum - optimalGain[pos + step];

            // Сохраняем максимум из возможных результатов
            bestCurrentMove = max(bestCurrentMove, result);
        }

        // Запоминаем лучший результат для текущей позиции
        optimalGain[pos] = bestCurrentMove;
    }

    // Побеждает первый игрок, если его итоговое преимущество положительное
    cout << "Результат: " << (optimalGain[0] > 0 ? 1 : 0) << endl;

    return 0; // Завершаем программу
}
