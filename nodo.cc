#include "nodo.h"

//Constructor damos las coordenadas x e y del nodo
Nodo::Nodo(int _x, int _y)
{
    x = _x;
    y = _y;
}
//Sobrecarga del constructor: ponemos a 0 las coordenadas
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
//Para cambiar el valor de las coordenadas del nodo
void Nodo::setNodo(int _x, int _y){
    x = _x;
    y = _y;
}