#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

void num_to_binary(double num, char *mas, int bits) {
    if (bits == 32) {
        float temp = (float)num;
        uint32_t raw;
        memcpy(&raw, &temp, sizeof(float));
        for (int i = bits - 1; i >= 0; i--) {
            mas[(bits - 1) - i] = ((raw >> i) & 1) + '0';
        }
    }
    else if (bits == 64) {
        uint64_t raw;
        memcpy(&raw, &num, sizeof(double));
        for (int i = bits - 1; i >= 0; i--) {
            mas[(bits - 1) - i] = ((raw >> i) & 1) + '0';
        }
    }
    mas[bits] = '\0';
}

double binary_to_num(const char *mas, int bits) {
    if (bits == 32) {
        uint32_t raw = 0;
        for (int i = 0; i < bits; i++) {
            raw = (raw << 1) | (mas[i] - '0');
        }
        float result;
        memcpy(&result, &raw, sizeof(float));
        return (double)result;
    }
    else if (bits == 64) {
        uint64_t raw = 0;
        for (int i = 0; i < bits; i++) {
            raw = (raw << 1) | (mas[i] - '0');
        }
        double result;
        memcpy(&result, &raw, sizeof(double));
        return result;
    }
    return 0.0;
}

double generate_number(double a, double b, int p) {
    double num = a + ((double)rand() / RAND_MAX) * (b - a);
    num = round(num * pow(10, p)) / pow(10, p);;
    return num;
}

void generate_variant(int variant, int k, int bits, double a, double b, int p) {
    char filename1[100], filename2[100];
    sprintf(filename1, "Задания/variant_%d.md", variant);
    sprintf(filename2, "Проверка/variant_%d.md", variant);
    
    FILE *f_task = fopen(filename1, "w");
    FILE *f_check = fopen(filename2, "w");
    
    if (!f_task || !f_check) {
        printf("Ошибка создания файла!");
        return;
    }
    
    fprintf(f_task, "№ п/п | Вещественное число\n");
    fprintf(f_check, "№ п/п | Вещественное число | Машинное представление | Точность\n");
    
    for (int i = 1; i <= k; i++) {
        double original = generate_number(a, b, p);
        
        char binary[bits + 1];
        num_to_binary(original, binary, bits);
        double converted = binary_to_num(binary, bits);
        double precision = fabs(original - converted);
        
        fprintf(f_task, "%5d | %*.*f\n", i, 6, p, original);
        fprintf(f_check, "%5d | %*.*f | %s | %.2e\n", i, 6, p, original, binary, precision);
    }
    
    fclose(f_task);
    fclose(f_check);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    
    FILE *config = fopen("config.txt", "r");
    if (!config) {
        printf("Ошибка: файл config.txt не найден!\n");
        return 1;
    }
    
    int n, k, bits, p;
    double a, b;
    
    char line[100];
    
    fscanf(config, "%d", &n);
    fscanf(config, "%d", &k);
    fscanf(config, "%d", &bits);
    fscanf(config, "%lf", &a);
    fscanf(config, "%lf", &b);
    fscanf(config, "%d", &p);
    
    fclose(config);
    
    printf("ГЕНЕРАТОР ИНДИВИДУАЛЬНЫХ ЗАДАНИЙ\n");
    printf("Вариантов: %d\n", n);
    printf("Заданий: %d\n", k);
    printf("Разрядность: %d бит\n", bits);
    printf("Диапазон: [%.2f, %.2f]\n", a, b);
    printf("Знаков после запятой: %d\n", p);
    
    mkdir("Задания");
    mkdir("Проверка");
    
    for (int variant = 1; variant <= n; variant++) {
        generate_variant(variant, k, bits, a, b, p);
    }
    
    return 0;
}