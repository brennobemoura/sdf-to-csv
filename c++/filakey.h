#ifndef FILAKEY_H
#define FILAKEY_H

#include <iostream>
using namespace std;

class FilaKey {
private:
    struct Node {
        string value;
        Node* next = 0;
    };

    Node* first;
    Node* last;

public:
    FilaKey();

    bool insert(string val);
    bool popFirst(string *val);
    int hasValue(string val);

    int length();

    void popAll();
};

#endif // FILAKEY_H
