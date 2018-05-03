#ifndef FILAVALUES_H
#define FILAVALUES_H

#include "filakey.h"

class FilaValues {
private:
    struct Node {
        FilaKey* fila = 0;
        Node* next = 0;
    };

    Node* first;
    Node* last;

public:
    FilaValues();

    bool insertLast(string val);
    bool insert(int ikey, string val);
    FilaKey* popFirst();
    bool popFirst(int ikey, string *val);

    int lengthValues(int ikey);

    void popAll();
};

#endif // FILAVALUES_H
