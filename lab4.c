#include <stdio.h>

#define ST 42                                                                                        
#define MAX 43

// Функция для сдвига битового массива на указанное количество позиций                               
void replace(int result[], int replace) {
    int res[MAX];  // Увеличиваем размер 
    for (int i = 0; i < MAX; i++) {
        if (i < replace)
            res[i] = result[i + ST - replace];
        else
            res[i] = result[i - replace];
    }

    // Копируем результат сдвига обратно в массив result
    for (int i = 0; i < MAX; i++) {
        result[i] = res[i];
    }
}

// Функция для вычисления автокорреляции
double correlat(int origin[], int move[], int length) {
    int matchCount = 0;

    // Подсчет числа совпадающих бит
    for (int i = 0; i < length; i++) {
        if (origin[i] == move[i]) {
            matchCount++;
        }
    }

    // Вычисление автокорреляции
    double p  = (double)1/(length);
    return (1/ST)*(matchCount - (length - matchCount));
    
}



int main() {

    // Исходные значения 1 задание
    int x1 = 1, x2 = 1, x3 = 0, x4 = 0, x5 = 0;
    int y1 = 1, y2 = 1, y3 = 1, y4 = 1, y5 = 1;

    // int x1 = 1, x2 = 1, x3 = 1, x4 = 1, x5 = 0;
    // int y1 = 1, y2 = 1, y3 = 0, y4 = 0, y5 = 0;

    // Массивы для хранения оригинальной и сдвинутой последовательностей
    int origin[MAX];
    int move[MAX];

    // Заполнение массива результатов операции XOR и сохранение оригинала
    for (int i = 0; i < MAX; i++) {
        origin[i] = x5 ^ y5;
        move[i] = x5 ^ y5;

        int sumx = x1 ^ x3;
        x5 = x4; x4 = x3; x3 = x2; x2 = x1;
        x1 = sumx;

        int sumy = y2 ^ y4;
        y5 = y4; y4 = y3; y3 = y2; y2 = y1;
        y1 = sumy;
    }

    // Вывод заголовка таблицы
    printf("Сдвиг| Биты     | Автокорреляция\n");

    // Вывод строк таблицы
    for (int shift = 0; shift < MAX; shift++) {
        printf("%4d | ", shift);

        // Вывод битов оригинала
        for (int i = 0; i < ST; i++) {
            printf("%d", move[i]);
        }

        printf(" | ");

        //  вычисление автокорреляции
        double corr = correlat(origin, move, ST);

        printf("%+1.3f\n", corr);
        // replace
        replace(move, 1);
    }

    return 0;
}
