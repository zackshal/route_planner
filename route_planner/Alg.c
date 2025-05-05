#include "Alg.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>

/* Внутренний узел для открытого множества */
typedef struct {
    int x, y;
} NodeRC;

/* Получить значение эвристики через current_h */
static double H(Point a, Point b) {
    return current_h ? current_h(a, b) : 0.0;
}

DLinkedList* a_star_search(int** cells, int width, int height,
    Point start, Point goal) {
    int sx = (int)start.x, sy = (int)start.y;
    int gx = (int)goal.x, gy = (int)goal.y;

    /* Проверка валидности старта/финиша */
    if (sx < 0 || sx >= width || sy < 0 || sy >= height ||
        gx < 0 || gx >= width || gy < 0 || gy >= height ||
        cells[sy][sx] || cells[gy][gx]) {
        fprintf(stderr, "Invalid or blocked start/goal\n");
        return NULL;
    }

    /* Выделяем g, f, came_from, закрытое и в-openSet */
    double** g = malloc(height * sizeof(double*));
    double** f = malloc(height * sizeof(double*));
    bool** closed = malloc(height * sizeof(bool*));
    bool** inOpen = malloc(height * sizeof(bool*));
    NodeRC** came = malloc(height * sizeof(NodeRC*));

    for (int y = 0; y < height; y++) {
        g[y] = malloc(width * sizeof(double));
        f[y] = malloc(width * sizeof(double));
        closed[y] = malloc(width * sizeof(bool));
        inOpen[y] = malloc(width * sizeof(bool));
        came[y] = malloc(width * sizeof(NodeRC));
        for (int x = 0; x < width; x++) {
            g[y][x] = f[y][x] = DBL_MAX;
            closed[y][x] = inOpen[y][x] = false;
        }
    }

    /* Простая структура openSet */
    NodeRC* openSet = malloc(width * height * sizeof(NodeRC));
    int openSize = 0;

    /* Инициализация для старта */
    g[sy][sx] = 0.0;
    f[sy][sx] = H(start, goal);
    openSet[openSize++] = (NodeRC){ sx, sy };
    inOpen[sy][sx] = true;

    /* 8 направлений (чётные — прямые, нечётные — диагонали) */
    int dirs[8][2] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    };

    bool found = false;
    while (openSize > 0) {
        /* Найти узел с min f */
        int best = 0;
        double bestF = f[openSet[0].y][openSet[0].x];
        for (int i = 1; i < openSize; i++) {
            double fi = f[openSet[i].y][openSet[i].x];
            if (fi < bestF) {
                bestF = fi; best = i;
            }
        }
        NodeRC cur = openSet[best];
        /* Удалить из openSet */
        inOpen[cur.y][cur.x] = false;
        openSet[best] = openSet[--openSize];

        /* Достигли цели? */
        if (cur.x == gx && cur.y == gy) {
            found = true;
            break;
        }
        closed[cur.y][cur.x] = true;

        /* Рассматриваем соседей */
        for (int d = 0; d < 8; d++) {
            int nx = cur.x + dirs[d][0];
            int ny = cur.y + dirs[d][1];
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
            if (cells[ny][nx]) continue;  /* блок */
            if (closed[ny][nx]) continue;

            /* Стоимость перехода */
            double cost = (d < 4 ? 1.0 : sqrt(2.0));
            double tentative = g[cur.y][cur.x] + cost;

            if (!inOpen[ny][nx] || tentative < g[ny][nx]) {
                came[ny][nx] = cur;
                g[ny][nx] = tentative;
                f[ny][nx] = tentative + H((Point) { nx, ny }, goal);
                if (!inOpen[ny][nx]) {
                    openSet[openSize++] = (NodeRC){ nx, ny };
                    inOpen[ny][nx] = true;
                }
            }
        }
    }

    /* Восстановление пути */
    DLinkedList* path = NULL;
    if (found) {
        path = list_create();
        NodeRC cur = { gx, gy };
        while (!(cur.x == sx && cur.y == sy)) {
            list_push_front(path, (Point) { cur.x, cur.y });
            cur = came[cur.y][cur.x];
        }
        list_push_front(path, start);
    }
    else {
        fprintf(stderr, "Path not found\n");
    }

    /* Освобождаем память */
    free(openSet);
    for (int y = 0; y < height; y++) {
        free(g[y]);      free(f[y]);
        free(closed[y]); free(inOpen[y]);
        free(came[y]);
    }
    free(g); free(f);
    free(closed); free(inOpen);
    free(came);

    return path;
}
