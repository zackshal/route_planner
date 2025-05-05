#ifndef VAR_STR_H
#define VAR_STR_H

#include <stdlib.h>

/* ����� */
typedef struct {
    double x, y;
} Point;

/* ���� ����������� ������ */
typedef struct Node {
    Point        data;
    struct Node* prev, * next;
} Node;

/* ������ ����� */
typedef struct {
    Node* head, * tail;
    size_t size;
} DLinkedList;

/* --- ������ ������ --- */
DLinkedList* list_create(void);
void         list_destroy(DLinkedList* lst);
void         list_push_back(DLinkedList* lst, Point p);
void         list_push_front(DLinkedList* lst, Point p);

/* ��������� */
typedef double (*Heuristic)(Point, Point);

/* ���������� (�����), p ������� � ������ */
double minkowski_distance(Point a, Point b, double p);
/* �����������: mat � �������� �������������� ������� 2?2 */
double mahalanobis_distance(Point a, Point b, const double mat[2][2]);

/* ������ ��� Heuristic */
double minkowski_euclid(Point a, Point b);
double mahalanobis_wrapper(Point a, Point b);
/* ������ inv-���������� ��� ������������ */
void   set_inverse_covariance(const double mat[2][2]);

/* ���������� ��������� */
extern Heuristic current_h;
void set_heuristic(Heuristic h);

#endif /* VAR_STR_H */
