#include <stdio.h>

#define ST 31
#define MAX 42

// Функция для сдвига битового массива на указанное количество позиций
void sdvig(int result[], int sdvig) {
    int res[MAX];  // Увеличиваем размер на 3 для хранения сдвинутых битов
    for (int i = 0; i < MAX; i++) {
        if (i < sdvig)
            res[i] = result[i + ST - sdvig];
        else
            res[i] = result[i - sdvig];
    }

    // Копируем результат сдвига обратно в массив result
    for (int i = 0; i < MAX; i++) {
        result[i] = res[i];
    }
}

// Функция для вычисления автокорреляции
double autocorrelation(int original[], int shifted[], int length, int shift) {
    int matchCount = 0;

    // Подсчет числа совпадающих бит
    for (int i = 0; i < length; i++) {
        if (original[i] == shifted[i]) {
            matchCount++;
        }
    }

    // Вычисление автокорреляции
    return (double)(matchCount - length / 2) / (length / 2);
}

int main() {

    // Исходные значения 1 задание
    int x1 = 1, x2 = 0, x3 = 0, x4 = 0, x5 = 0;
    int y1 = 1, y2 = 1, y3 = 0, y4 = 1, y5 = 1;

    
    // int x1 = 1, x2 = 1, x3 = 0, x4 = 0, x5 = 1;
    // int y1 = 1, y2 = 1, y3 = 1, y4 = 0, y5 = 0;

    // Массивы для хранения оригинальной и сдвинутой последовательностей
    int original[MAX];
    int shifted[MAX];

    // Заполнение массива результатов операции XOR и сохранение оригинала
    for (int i = 0; i < MAX; i++) {
        original[i] = x5 ^ y5;
        shifted[i] = x5 ^ y5;

        int sumx = x3 ^ x4;
        x5 = x4; x4 = x3; x3 = x2; x2 = x1;
        x1 = sumx;

        int sumy = y2 ^ y4;
        y5 = y4; y4 = y3; y3 = y2; y2 = y1;
        y1 = sumy;
    }

    // Вывод заголовка таблицы
    printf("Сдвиг|                     Биты                   | Автокорреляция\n");

    // Вывод строк таблицы
    for (int shift = 0; shift < MAX; shift++) {
        printf("%4d | ", shift);

        // Вывод битов оригинала
        for (int i = 0; i < ST; i++) {
            printf("%d", shifted[i]);
        }

        printf(" | ");

        // Сдвиг и вычисление автокорреляции
        double corr = autocorrelation(original, shifted, ST, shift);

        printf("%+1.3f\n", corr);

        sdvig(shifted, 1);
    }

    return 0;
}
