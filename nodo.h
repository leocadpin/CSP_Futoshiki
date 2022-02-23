#ifndef _NODO_
#define _NODO_

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