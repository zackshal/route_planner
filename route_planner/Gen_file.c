#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Var_str.h"
#include "Alg.h"
#include "File_out.h"

/* Генерация карты с отрезками-препятствиями */
int** generate_map(int width, int height, int obstacles_count) {
    int** map = malloc(height * sizeof(int*));
    for (int y = 0; y < height; y++)
        map[y] = calloc(width, sizeof(int));

    int placed = 0, tries = 0;
    int maxLen = (width > height ? width : height) / 3;
    if (maxLen < 1) maxLen = 1;

    while (placed < obstacles_count && tries < obstacles_count * 100) {
        tries++;
        int ori = rand() % 4;
        int dx = 0, dy = 0;
        switch (ori) {
        case 0: dx = 1; dy = 0; break;  // горизонталь
        case 1: dx = 0; dy = 1; break;  // вертикаль
        case 2: dx = 1; dy = 1; break;  // диаг. вниз-вправо
        case 3: dx = 1; dy = -1; break; // диаг. вверх-вправо
        }
        int len = rand() % maxLen + 1;

        // Отрезок не помещается?
        if ((dx && len > width) || (dy && len > height))
            continue;

        int x0, y0;
        if (dx == 1 && dy == 0) {
            x0 = rand() % (width - len + 1);
            y0 = rand() % height;
        }
        else if (dx == 0 && dy == 1) {
            x0 = rand() % width;
            y0 = rand() % (height - len + 1);
        }
        else if (dx == 1 && dy == 1) {
            x0 = rand() % (width - len + 1);
            y0 = rand() % (height - len + 1);
        }
        else {
            x0 = rand() % (width - len + 1);
            y0 = rand() % (height - len + 1) + (len - 1);
        }

        // Проверка на пересечения и «коридор» вокруг
        int ok = 1;
        for (int i = 0; i < len; i++) {
            int x = x0 + dx * i;
            int y = y0 + dy * i;
            if (map[y][x]) { ok = 0; break; }
            for (int yy = y - 1; yy <= y + 1; yy++)
                for (int xx = x - 1; xx <= x + 1; xx++)
                    if (xx >= 0 && xx < width && yy >= 0 && yy < height && map[yy][xx])
                        ok = 0;
            if (!ok) break;
        }
        if (!ok) continue;

        // Помечаем препятствие
        for (int i = 0; i < len; i++) {
            map[y0 + dy * i][x0 + dx * i] = 1;
        }
        placed++;
    }

    return map;
}

/* Освободить память карты */
void free_map(int** map, int height) {
    for (int y = 0; y < height; y++)
        free(map[y]);
    free(map);
}

/* Запись карты со строковыми переводами */
void write_map(const char* fname, int** map, int width, int height) {
    FILE* f = fopen(fname, "w");
    if (!f) return;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            fputc(map[y][x] ? '1' : '0', f);
        fputc('\n', f);
    }
    fclose(f);
}

/* Запись карты без строковых переводов (flat) */
void write_map_flat(const char* fname, int** map, int width, int height) {
    FILE* f = fopen(fname, "w");
    if (!f) return;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            fputc(map[y][x] ? '1' : '0', f);
    fclose(f);
}

int main(int argc, char* argv[]) {
    // Настройка консоли для корректного вывода русского текста
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    int width, height, obstacles;
    char line[256];

    // Ввод ширины, высоты и количества препятствий 
    printf("Введите width, height и obstacles_count (через пробел или запятую): ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Ошибка чтения ввода.\n");
        return 1;
    }
    // заменить все запятые на пробелы
    for (char* p = line; *p; ++p) if (*p == ',') *p = ' ';
    if (sscanf(line, "%d %d %d", &width, &height, &obstacles) != 3) {
        fprintf(stderr, "Неверный формат. Введите три целых числа.\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    int** map = generate_map(width, height, obstacles);

    // Выбор формата записи в файл 
    printf("Записать карту с переводами строк? (1 — да, 0 — нет): ");
    if (!fgets(line, sizeof(line), stdin)) line[0] = '1';
    int choice = atoi(line);
    if (choice) {
        write_map("map.txt", map, width, height);
    }
    else {
        write_map_flat("map.txt", map, width, height);
    }

    // Ввод точки A 
    Point A, B;
    printf("Введите начальную точку A (x y через пробел или запятую): ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Ошибка чтения координат A.\n");
        return 1;
    }
    for (char* p = line; *p; ++p) if (*p == ',') *p = ' ';
    if (sscanf(line, "%lf %lf", &A.x, &A.y) != 2) {
        fprintf(stderr, "Неверный формат для A. Должны быть два числа.\n");
        return 1;
    }

    // Ввод точки B 
    printf("Введите конечную точку B (x y через пробел или запятую): ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Ошибка чтения координат B.\n");
        return 1;
    }
    for (char* p = line; *p; ++p) if (*p == ',') *p = ' ';
    if (sscanf(line, "%lf %lf", &B.x, &B.y) != 2) {
        fprintf(stderr, "Неверный формат для B. Должны быть два числа.\n");
        return 1;
    }

    // Выбор эвристики 
    printf("Выберите эвристику:\n"
        " 1) Минковский\n"
        " 2) Махаланобис\n"
        "Ваш выбор: ");
    if (!fgets(line, sizeof(line), stdin)) line[0] = '1';
    int heur = atoi(line);
    if (heur == 2) {
        double a11, a12, a21, a22;
        printf("Введите a11 a12 a21 a22 (inv-covariance): ");
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "Ошибка чтения матрицы.\n");
            return 1;
        }
        for (char* p = line; *p; ++p) if (*p == ',') *p = ' ';
        if (sscanf(line, "%lf %lf %lf %lf", &a11, &a12, &a21, &a22) != 4) {
            fprintf(stderr, "Неверный формат матрицы.\n");
            return 1;
        }
        double m[2][2] = { {a11,a12},{a21,a22} };
        set_inverse_covariance(m);
        set_heuristic(mahalanobis_wrapper);
    }
    else {
        set_heuristic(minkowski_euclid);
    }

    // Поиск пути и вывод 
    DLinkedList* path = a_star_search(map, width, height, A, B);
    print_map_with_path(map, width, height, path);

    // Освобождение ресурсов 
    free_map(map, height);
    list_destroy(path);
    return 0;
}
