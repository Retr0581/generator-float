#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

void num_to_binary(long double num, char *mas, int bits) {
    if (bits == 32) {
        float temp = (float)num;
        uint32_t raw;
        memcpy(&raw, &temp, sizeof(float));
        for (int i = bits - 1; i >= 0; i--) {
            mas[(bits - 1) - i] = ((raw >> i) & 1) + '0';
        }
    }
    else if (bits == 64) {
        double temp = (double)num;
        uint64_t raw;
        memcpy(&raw, &temp, sizeof(double));
        for (int i = bits - 1; i >= 0; i--) {
            mas[(bits - 1) - i] = ((raw >> i) & 1) + '0';
        }
    }
    else if (bits == 128){
        uint8_t raw[16];
        memcpy(raw, &num, sizeof(long double));
        int pos = 0;
        for (int byte = 15; byte >= 0; byte--)
            for (int bit = 7; bit >= 0; bit--)
                mas[pos++] = ((raw[byte] >> bit) & 1) + '0';
    }

    mas[bits] = '\0';
}

long double binary_to_num(const char *mas, int bits) {
    if (bits == 32) {
        uint32_t raw = 0;
        for (int i = 0; i < bits; i++) {
            raw = (raw << 1) | (mas[i] - '0');
        }
        float result;
        memcpy(&result, &raw, sizeof(float));
        return (long double)result;
    }
    else if (bits == 64) {
        uint64_t raw = 0;
        for (int i = 0; i < bits; i++) {
            raw = (raw << 1) | (mas[i] - '0');
        }
        double result;
        memcpy(&result, &raw, sizeof(double));
        return (long double)result;
    }
    else if (bits == 128){
        uint8_t raw[16] = {0};
        int pos = 0;
        for (int byte = 15; byte >= 0; byte--)
            for (int bit = 7; bit >= 0; bit--)
                raw[byte] |= (mas[pos++] - '0') << bit;
        long double result;
        memcpy(&result, raw, sizeof(long double));

        return result;
    }
    return 0.0L;
}

long double generate_number(long double a, long double b, int p) {
    long double num = a + ((long double)rand() / RAND_MAX) * (b - a);
    num = roundl(num * powl(10.0L, p)) / powl(10.0L, p);
    return num;
}

void generate_variant(int variant, int k, int bits, long double a, long double b, int p) {
    char filename1[100], filename2[100];
    sprintf(filename1, "�������/�������_%d.md", variant);
    sprintf(filename2, "��������/�������_%d.md", variant);
    
    FILE *f_task = fopen(filename1, "w");
    FILE *f_check = fopen(filename2, "w");
    
    if (!f_task || !f_check) {
        printf("������ �������� �����!");
        return;
    }
    
    fprintf(f_task, "� �/� | ������������ �����\n");
    fprintf(f_check, "� �/� | ������������ ����� | �������� ������������� | ��������\n");
    
    for (int i = 1; i <= k; i++) {
        long double original = generate_number(a, b, p);
        
        char binary[bits + 1];
        num_to_binary(original, binary, bits);
        long double converted = binary_to_num(binary, bits);
        long double precision = fabsl(original - converted);
        
        fprintf(f_task, "%5d | %*.*Lf\n", i, 6, p, original);
        fprintf(f_check, "%5d | %*.*Lf | %s | %.2Le\n", i, 6, p, original, binary, precision);
    }
    
    fclose(f_task);
    fclose(f_check);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    
    FILE *config = fopen("config.txt", "r");
    if (!config) {
        printf("������: ���� config.txt �� ������!\n");
        return 1;
    }
    
    int n, k, bits, p;
    long double a, b;
    
    fscanf(config, "%d", &n);
    fscanf(config, "%d", &k);
    fscanf(config, "%d", &bits);
    fscanf(config, "%Lf", &a);
    fscanf(config, "%Lf", &b);
    fscanf(config, "%d", &p);
    
    fclose(config);
    
    printf("��������� �������������� �������\n");
    printf("���������: %d\n", n);
    printf("�������: %d\n", k);
    printf("�����������: %d ���\n", bits);
    printf("��������: [%.2Lf, %.2Lf]\n", a, b);
    printf("������ ����� �������: %d\n", p);
    
    mkdir("�������");
    mkdir("��������");
    
    for (int variant = 1; variant <= n; variant++) {
        generate_variant(variant, k, bits, a, b, p);
    }
    
    return 0;
}