#ifndef VAR_STR_H
#define VAR_STR_H

#include <stdlib.h>

/* Точка */
typedef struct {
    double x, y;
} Point;

/* Узел двусвязного списка */
typedef struct Node {
    Point        data;
    struct Node* prev, * next;
} Node;

/* Список точек */
typedef struct {
    Node* head, * tail;
    size_t size;
} DLinkedList;

/* --- Методы списка --- */
DLinkedList* list_create(void);
void         list_destroy(DLinkedList* lst);
void         list_push_back(DLinkedList* lst, Point p);
void         list_push_front(DLinkedList* lst, Point p);

/* Эвристики */
typedef double (*Heuristic)(Point, Point);

/* Минковский (общий), p задаётся в обёртке */
double minkowski_distance(Point a, Point b, double p);
/* Махаланобис: mat — обратная ковариационная матрица 2?2 */
double mahalanobis_distance(Point a, Point b, const double mat[2][2]);

/* Обёртки под Heuristic */
double minkowski_euclid(Point a, Point b);
double mahalanobis_wrapper(Point a, Point b);
/* Задать inv-ковариацию для махаланобиса */
void   set_inverse_covariance(const double mat[2][2]);

/* Глобальная эвристика */
extern Heuristic current_h;
void set_heuristic(Heuristic h);

#endif /* VAR_STR_H */
