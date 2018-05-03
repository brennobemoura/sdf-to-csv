#include "filakey.h"

FilaKey::FilaKey() {
    first = 0;
    last = 0;
}

bool FilaKey::insert(string val) {
    if (!this->first) {
        this->first = this->last = new Node();
        this->first->value = val;
        return true;
    }

    this->last = this->last->next = new Node();
    this->last->value = val;
    return true;
}

bool FilaKey::popFirst(string *val) {
    if (!this->first)
        return false;

    *val = this->first->value;

    Node*aux = this->first->next;
    delete this->first;
    first = aux;

    if (!first)
        last = 0;

    return true;
}

int FilaKey::hasValue(string val) {
    if (!this->first)
        return -1;

    Node* aux = this->first;
    for (int i = 0; aux; i++) {
        if (aux->value == val)
            return i;
        aux = aux->next;
    }
    return -1;
}

int FilaKey::length() {
    Node*aux = this->first;
    int sum = 0;
    for (; aux; sum++)
        aux = aux->next;

    return sum;
}

void FilaKey::popAll() {
    if (!this->first)
        return;

    Node*aux = 0;
    while(this->first) {
        aux = this->first->next;
        delete this->first;
        this->first = aux;
    }

    this->last = this->first;
}
