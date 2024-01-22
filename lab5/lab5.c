#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define N_LEN 300   //250
#define G_LEN 5
int main(){
     srand(time(NULL));

    int N[N_LEN];
    int N_c[N_LEN];
    int N_Base[N_LEN];
    int G[G_LEN] = {1, 0, 1, 1, 1};
    
    // Заполнение массива данных 
    for(int i = 0; i<N_LEN-G_LEN+1; i++){
        N[i] = rand() % 2;
        N_c[i] = N[i];
        N_Base[i] = N[i];
    }
    // Добавление нулей в конец
    for(int i = N_LEN-G_LEN+1; i < N_LEN; i++) N[i] = 0;
    
    // Вывод N
    for(int i=0; i < N_LEN; i++) printf("%d", N[i]);
    printf(" <- N\n");
    
    // Вычисление XOR и остатка от деления
    for(int i=0; i < N_LEN-G_LEN+1; i++){
        if(N[i] == 1){
            for(int j=0; j < G_LEN; j++) 
            N[i+j] ^= G[j];
        }
    }
    //Вывод остатка при отправке
    for(int i=N_LEN-G_LEN+1; i < N_LEN; i++) printf("%d", N[i]);
    printf(" <- CRC \n\n");

    // Добавление остатка
    for(int i=N_LEN-G_LEN+1; i < N_LEN; i++){
    N_c[i] = N[i];
    N_Base[i] = N[i];
    }
    //Вывод N при получении
    for(int i=0; i < N_LEN; i++) printf("%d", N_c[i]);
    printf(" <- N_c\n");

     // опять подсчет XOR и остатка от деления    
    for(int i=0; i < N_LEN-G_LEN+1; i++){
            if(N_c[i] == 1){
                for(int j=0; j < G_LEN; j++)
                N_c[i+j] ^= G[j];
            }
    }

    for(int i=N_LEN-G_LEN+1; i < N_LEN; i++) printf("%d", N_c[i]);
    printf(" <- CRC\n\n");



    // Искажение
    int total_k = 0;
    int iter = 1000;

    for(int len_n = 100; len_n < 2000; len_n += 50){
        int kk=0;
        for(int package = 0; package < 50; package++){
            int No[len_n];
            int No_c[len_n];
            for(int i = 0; i<len_n-G_LEN+1; i++){
                No[i] = rand() % 2;
                No_c[i] = No[i];
                // N_Base[i] = N[i];
            }
            // Добавление нулей в конец
            for(int i = len_n-G_LEN+1; i < len_n; i++) No[i] = 0;
            // for(int i = 0; i < len_n; i++) printf("%d", No[i]);
            // printf("\n");
            // Вычисление XOR и остатка от деления
            for(int i=0; i < len_n-G_LEN+1; i++){
                if(No[i] == 1){
                    for(int j=0; j < G_LEN; j++) 
                    No[i+j] ^= G[j];
                 }
            }
            // Добавление остатка
            for(int i=len_n-G_LEN+1; i < len_n; i++){
            No_c[i] = No[i];
            }
            // for(int i = 0; i < len_n; i++) printf("%d", No_c[i]);
            // printf("^^\n");
            int k=0;
            // Получился готовый массив No_c вместе с CRC
            for(int o = 0; o<len_n; o++){
                int No_cc[len_n];
                for(int i = 0; i<len_n; i++)
                    No_cc[i] = No_c[i];
                // Искажение битов по очереди
                if (No_cc[o] == 1) No_cc[o] = 0;
                else No_cc[o] = 1;
                // Искажение рандомных битов
                if (No_cc[rand()%len_n] == 1) No_cc[rand()%len_n] = 0;
                else No_cc[rand()%len_n] = 1;

                // Вычисление XOR и остатка от деления
                for(int i=0; i < len_n-G_LEN+1; i++){
                    if(No_cc[i] == 1){
                        for(int j=0; j < G_LEN; j++)
                        No_cc[i+j] ^= G[j];
                    }
                }
                // Проверка на ошибки
                for(int i = len_n-G_LEN+1; i<len_n; i++){
                    if(No_cc[i] == 1) break;
                    if((i == len_n-1) & (No_cc[i] == 0)) k++;
                }
            }
            // if (len_n == 100){
            //     printf("k=%d ", k);
            // }
            //printf("k=%d ", k);
            kk += k;
        }
        double err = (double)kk/(double)(len_n*50);
        fflush(stdout);
        //printf("N_LEN = %d | err = %f\n", len_n, err);
        printf("%f, ", err);
    }

    return 0;
}
