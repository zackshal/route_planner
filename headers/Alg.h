#pragma once
#ifndef ALG_H
#define ALG_H

#include "Var_str.h"

/* A*-поиск на сetке 0/1 */
DLinkedList* a_star_search(
    int** cells,
    int width,
    int height,
    Point start,
    Point goal
);

#endif /* ALG_H */
