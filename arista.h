#ifndef _ARISTA_
#define _ARISTA_
#include "nodo.cc"

class Arista
{
private:
    /* data */
    Nodo n1;
    Nodo n2;
public:
    Arista(Nodo, Nodo);
    ~Arista();
    Nodo get_n2();
    Nodo get_n1();
    void imprimirArista();
};

#endif