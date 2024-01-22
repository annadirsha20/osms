import matplotlib.pyplot as plt
import numpy as np


def crc_fun(list, add_zeros):
    G = [1, 1, 1, 1, 0, 1, 1, 1] # Порождающий полином G
    if add_zeros == True:
        # Добавляем 7 нулей в конец
        list = np.concatenate((list, np.zeros(7, dtype=int)))
    else:
        list = np.array(list)
        
    for i in range(len(list)-8):
        if list[i] == 1 :
            list[i:i+8] ^= G
    
    return list[-7:]

def gold_sic():
    # 24 вариант
    x = [1, 1, 0, 0, 0]
    y = [1, 1, 1, 1, 1]
    gold_a = []
    for i in range(31):
        gold_a.append(x[4] ^ y[4])
        x = [x[3] ^ x[4]] + x[:4]
        y = [y[1] ^ y[4]] + y[:4]
        
    return gold_a

def correlate_rx(list, gold):
    gold = np.repeat(gold, 8)
    corrarr = np.correlate(list, gold, "valid")
    corrarr = corrarr/np.amax(corrarr)
    corr = np.argmax(corrarr)
    list = list[corr:]
    plt.figure(5)
    plt.xlabel('Номер элемента')
    plt.ylabel('Значение Корреляции')
    plt.title('Корреляция')
    plt.plot(corrarr) 
    plt.show()
    return list

def decryp(list):
    dec = []
    for i in range(0, len(list), 8):
        average = np.mean(list[i:i+8]) # среднее среди 8 отсчётов
        if average > 0.5:
            dec.append(1)
        else:
            dec.append(0)
    
    return dec


# Ввод имени и фамилии
name = input("Введите имя: ")
lname = input("Введите фамилию: ")

# Кодирование в ASCII
bi_nposl = ''.join(format(ord(char), '08b') for char in name + ' ' + lname) # пробел между И и Ф
bi_nposl = np.array([int(bit) for bit in bi_nposl])

plt.figure(1,figsize=(15,7))
plt.subplots_adjust(hspace=0.3, wspace=0.3)
plt.subplot(231)
plt.xlabel('Биты')
plt.ylabel('Амплитуда')
plt.title('Имя и Фамилия')
plt.plot(bi_nposl)

print('Битовая послед -', bi_nposl)

# CRC
crc = crc_fun(bi_nposl, True)
print('Crc -', crc)

# Голд
gold = gold_sic()
print('Голд -', np.array(gold)) # Numpy чтобы выводилось без запятых

Nx = np.concatenate((gold, bi_nposl, crc)) # обьединение

plt.subplot(232)
plt.xlabel('Биты')
plt.ylabel('Амплитуда')
plt.title('Голд, данные, crc')
plt.plot(Nx)

# Преобразование во временные отсчёты
Nx_rep = np.repeat(Nx, 8) # 8 отчётов на 1 бит

plt.subplot(233)
plt.xlabel('Временые отсчёты')
plt.ylabel('Амплитуда')
plt.title('Голд, данные, crc(repeat 8)')
plt.plot(Nx_rep)

# Нулевой массив х2
Nx_2 = np.zeros(len(Nx_rep)*2) # создаём удвоенный массив

start_sig = int(input("Введите число до {}: ".format(len(Nx_rep))))
Nx_2[start_sig:start_sig + len(Nx_rep)] = Nx_rep # вносим всё в массив удвоенной длинны

plt.subplot(234)
plt.xlabel('Время(1сек)')
plt.ylabel('Амплитуда')
plt.title('Сигнал')
plt.plot(Nx_2)

# Создание и добавление шума
noise = np.random.normal(0, 0.3, len(Nx_2)) # шум в пределе ± 0.3 от нуля
signal = noise + Nx_2

plt.subplot(235)
plt.xlabel('Время(1сек)')
plt.ylabel('Амплитуда')
plt.title('Сигнал вместе с шумом')
plt.plot(signal)

# Корреляционный прием
signal = correlate_rx(signal, gold)

plt.figure(2,figsize=(6,6))
plt.subplots_adjust(hspace=0.5)
plt.subplot(211)
plt.xlabel('Время(<1сек)')
plt.ylabel('Амплитуда')
plt.title('Сигнал начиная с синхронизации')
plt.plot(signal)

# Убрать начало и повторы, декодировать
signal = signal[:len(Nx_rep)] # Обрезаем лишний сигнал
decr = decryp(signal)

print('Сигнал -', np.array(decr))

plt.subplot(212)
plt.xlabel('Время(<1сек)')
plt.ylabel('Амплитуда')
plt.title('Сигнал без repeat')
plt.plot(signal)

# Убрать GOLD
decr = decr[len(gold):]

# Проверка всё ли верно передалось
crc_sig = crc_fun(decr, False)
print('Проверка crc -',crc_sig)
if 1 in crc_sig:
    print("ОШИБКА")

# Убирать CRC и преобразовать в строку
decr = decr[:-7]
# Преобразование массива бит в строку ASCII
decod_str = ''.join([chr(int(''.join(map(str, byte)), 2)) for byte in np.array_split(decr, len(decr)//8)])
print(decod_str)
print('\n')


### 13 
# Переданный и полученый
fft1 = abs(np.fft.fftshift(np.fft.fft(Nx_rep))/len(Nx_rep) + 0.1)
fft2 = abs(np.fft.fftshift(np.fft.fft(signal))/len(signal))
t = np.arange(-len(fft1)/2, len(fft1)/2)
len(Nx_rep)/2
len(fft1)/2
plt.figure(3)
plt.xlabel('Частота[Гц]')
plt.ylabel('Амплитуда')
plt.title('Спектры получ и перед сигнала')
plt.plot(t, fft1, label = 'Переданный') 
plt.plot(t, fft2, label = 'Полученый')
plt.legend()

# Создание сигналов разной длительности
rep_4 = np.repeat(Nx, 4)
rep_8 = np.repeat(Nx, 8)
rep_16 = np.repeat(Nx, 16)

# одинаковая длинная
rep_8 = rep_8[:len(rep_4)]
rep_16 = rep_16[:len(rep_4)]

# делим на длинну массива для нормировния
fft_4 = abs(np.fft.fftshift(np.fft.fft(rep_4)/len(rep_4))) * 1.5 # умножение для наглядности графиков
fft_8 = abs(np.fft.fftshift(np.fft.fft(rep_8)/len(rep_8))) * 1.2
fft_16 = abs(np.fft.fftshift(np.fft.fft(rep_16)/len(rep_16))) 

t = np.arange(-len(fft_4)/2, len(fft_4)/2)
plt.figure(4)
plt.xlabel('Частота[Гц]')
plt.ylabel('Амплитуда')
plt.title('Спектры 3 разных сигналов')
plt.ylim(-0.01, 0.2)
plt.plot(t,fft_4) 
plt.plot(t,fft_8)
plt.plot(t,fft_16)
plt.show()

print(len(fft1)/2)
