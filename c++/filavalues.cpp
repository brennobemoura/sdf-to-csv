#include "filavalues.h"

FilaValues::FilaValues() {
   this->first = this->last = 0;
}

//bool FilaValues::insertLast(string val) {
//    if (!this->first) {
//        this->last = this->first = new Node();
//        (first->fila = new FilaKey())->insert(val);
//        return true;
//    }

//    this->last = this->last->next = new Node();
//    this->last
//}

bool FilaValues::insert(int ikey, string val) {
    if (!this->first) {
        this->last = this->first = new Node();
        return (first->fila = new FilaKey())->insert(val);
    }

    Node*aux = this->first;
    int ival = 0;
    for(;aux->next && ival < ikey; ival++)
        aux = aux->next;

    if (ival == ikey) {
        return aux->fila->insert(val);
    }

    aux = this->last = aux->next = new Node();
    return (aux->fila = new FilaKey())->insert(val);
}

FilaKey* FilaValues::popFirst() {
    if (!this->first)
        return 0;

    Node*aux = this->first->next;
    FilaKey* fila = first->fila;
    delete this->first;

    this->first = aux;
    if (!first)
        last = 0;

    return fila;
}

bool FilaValues::popFirst(int ikey, string *val) {
    if (!this->first)
        return false;

    Node*aux = this->first;
    int ival = 0;
    for(;aux->next && ival < ikey; ival++)
        aux = aux->next;

    if (ival == ikey)
        return aux->fila->popFirst(val);

    return false;
}

int FilaValues::lengthValues(int ikey) {
    if (!this->first)
        return 0;

    Node*aux = this->first;
    int ival = 0;
    for(;aux->next && ival < ikey; ival++)
        aux = aux->next;

    if (ival == ikey)
        return aux->fila->length();

    return 0;
}

void FilaValues::popAll() {
    if (!this->first)
        return;

    Node*aux = 0;
    while (this->first) {
        aux = this->first->next;
        this->first->fila->popAll();
        delete this->first->fila;
        delete this->first;
        this->first = aux;
    }

    this->last = this->first;
}
