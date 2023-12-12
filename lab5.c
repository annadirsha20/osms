#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define N_LEN 200   //250
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
    for(int u = 0; u<iter; u++){
        int k=0;
        for(int o = 0; o<N_LEN; o++){
            int N_cc[N_LEN];
            for(int i = 0; i<N_LEN; i++)
            N_cc[i] = N_Base[i];
            // Искажение битов по очереди
            if (N_cc[o] == 1) N_cc[o] = 0;
            else N_cc[o] = 1;
        
            // Искажение рандомных битов
            if (N_cc[rand()%N_LEN] == 1) N_cc[rand()%N_LEN] = 0;
            else N_cc[rand()%N_LEN] = 1;

            // Вычисление XOR и остатка от деления
            for(int i=0; i < N_LEN-G_LEN+1; i++){
                if(N_cc[i] == 1){
                    for(int j=0; j < G_LEN; j++)
                    N_cc[i+j] ^= G[j];
                }
            }
            // Проверка на ошибки
            for(int i = N_LEN-G_LEN+1; i<N_LEN; i++){
                if(N_cc[i] == 1) break;
                if((i == N_LEN-1) & (N_cc[i] == 0)) k++;
            }
        }
        total_k += k;
        // printf("%d,", k);
    }
    printf("Среднее количество ошибок = %f\n", (float)total_k/iter);
    printf("\n");
    return 0;
}
