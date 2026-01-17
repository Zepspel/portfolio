//
// Created by pavel on 30.11.2025.
//

#include "Table/tableIterator.h"

int main() {

    static_assert(std::bidirectional_iterator<TableIterator<int, int, true>>);
    static_assert(std::bidirectional_iterator<TableIterator<int, int, false>>);





    return 0;
}