#ifndef _NODO_
#define _NODO_

//CLASE NODO: LA USAMOS PARA REPRESENTAR LA ESTRUCTURA DE UN NODO 
//(en esencia, un punto con coordenadas x y)
class Nodo
{
private:
    /* data */
    int x;
    int y;
    
public:
    Nodo(int, int);
    Nodo();
    ~Nodo();
    void setNodo(int, int);
    int get_x();
    int get_y();
};

#endif