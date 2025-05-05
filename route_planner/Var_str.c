#include "Var_str.h"
#include <math.h>

/* Внутренняя матрица inv_cov для Mahalanobis */
static double inv_cov_mat[2][2] = { {1, 0}, {0, 1} };

void set_inverse_covariance(const double mat[2][2]) {
    inv_cov_mat[0][0] = mat[0][0];
    inv_cov_mat[0][1] = mat[0][1];
    inv_cov_mat[1][0] = mat[1][0];
    inv_cov_mat[1][1] = mat[1][1];
}

double minkowski_distance(Point a, Point b, double p) {
    return pow(
        pow(fabs(a.x - b.x), p) +
        pow(fabs(a.y - b.y), p),
        1.0 / p
    );
}

double mahalanobis_distance(Point a, Point b, const double inv_cov[2][2]) {
    double dx = a.x - b.x, dy = a.y - b.y;
    double m2 = dx * (inv_cov[0][0] * dx + inv_cov[0][1] * dy)
        + dy * (inv_cov[1][0] * dx + inv_cov[1][1] * dy);
    return sqrt(m2);
}

/* Обёртки под сигнатуру Heuristic */
double minkowski_euclid(Point a, Point b) {
    return minkowski_distance(a, b, 2.0);  /* p=2 ? Евклид */
}

double mahalanobis_wrapper(Point a, Point b) {
    return mahalanobis_distance(a, b, inv_cov_mat);
}

/*  Реализация двунаправленного списка */
static Node* node_create(Point p) {
    Node* n = malloc(sizeof(Node));
    n->data = p;
    n->prev = n->next = NULL;
    return n;
}

DLinkedList* list_create(void) {
    DLinkedList* lst = malloc(sizeof(DLinkedList));
    lst->head = lst->tail = NULL;
    lst->size = 0;
    return lst;
}

void list_destroy(DLinkedList* lst) {
    if (!lst) return;
    Node* cur = lst->head;
    while (cur) {
        Node* nx = cur->next;
        free(cur);
        cur = nx;
    }
    free(lst);
}

void list_push_back(DLinkedList* lst, Point p) {
    Node* n = node_create(p);
    if (!lst->tail) {
        lst->head = lst->tail = n;
    }
    else {
        lst->tail->next = n;
        n->prev = lst->tail;
        lst->tail = n;
    }
    lst->size++;
}

void list_push_front(DLinkedList* lst, Point p) {
    Node* n = node_create(p);
    if (!lst->head) {
        lst->head = lst->tail = n;
    }
    else {
        lst->head->prev = n;
        n->next = lst->head;
        lst->head = n;
    }
    lst->size++;
}

/* Глобальная эвристика */
Heuristic current_h = NULL;
void set_heuristic(Heuristic h) {
    current_h = h;
}
