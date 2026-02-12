#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <math.h>

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

int main() {
    setlocale(LC_ALL, "");

    int bits;
    printf("Выберите битность 32 или 64: ");
    scanf("%d", &bits);
    char bin[bits];

    double num;
    printf("Введите число: ");
    scanf("%lf", &num);

    num_to_binary(num, bin, bits);
    double back = binary_to_num(bin, bits);
    printf("%s\n", bin);
    printf("%.10f (точность: %.2e)\n", back, fabs(num - back));
    
    return 0;
}