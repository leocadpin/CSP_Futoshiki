#include "nodo.h"


Nodo::Nodo(int _x, int _y)
{
    x = _x;
    y = _y;


}

Nodo::Nodo(){
    x=0;
    y=0;
}    

Nodo::~Nodo()
{

}

int Nodo::get_x(){
    return x;
}

int Nodo::get_y(){
    return y;
}

void Nodo::setNodo(int _x, int _y){
    x = _x;
    y = _y;
}