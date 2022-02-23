#include "arista.h"
#include <iostream>

using namespace std;

Arista::Arista(Nodo a, Nodo b){

    n1 = Nodo(a.get_x() , a.get_y());

    n2 = Nodo(b.get_x(), b.get_y());

}

Arista::~Arista(){
    
}

void Arista::imprimirArista(){

    cout << "Esta arista tiene los nodos con coordenadas: " << endl;
    cout << "A: " << n1.get_x() << ", " << n1.get_y() << endl;
    cout << "B: " << n2.get_x() << ", " << n2.get_y() << endl;
}