#include <stdio.h>
#include <stdlib.h>
#include "../headers/Var_str.h"

void print_map_with_path(int** cells,
    int width,
    int height,
    DLinkedList* path)
{
    /* Вспомогательный буфер */
    char** disp = malloc(height * sizeof(char*));
    for (int y = 0; y < height; y++) {
        disp[y] = malloc(width + 1);
        for (int x = 0; x < width; x++) {
            disp[y][x] = (cells[y][x] ? '#' : '.');
        }
        disp[y][width] = '\0';
    }

    /* Отметим маршрут */
    if (path && path->head) {
        /* 'A' */
        disp[(int)path->head->data.y]
            [(int)path->head->data.x] = 'A';
        /* 'o' по середине и 'B' в конце */
        for (Node* n = path->head->next; n; n = n->next) {
            char mark = (n->next ? 'o' : 'B');
            disp[(int)n->data.y][(int)n->data.x] = mark;
        }
    }

    /* Печать и очистка */
    for (int y = 0; y < height; y++) {
        printf("%s\n", disp[y]);
        free(disp[y]);
    }
    free(disp);
}
