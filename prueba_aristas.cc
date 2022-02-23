#include "arista.cc"
#include "matdominios.cc"
#include <queue>


int main(){

    int num = 4;
    int contador=0;
    Nodo mat[num][num];
    
    queue< Arista > cola_aristas;

    for(int i=0; i < num; i++){
        for(int j=0; j < num; j++){
            mat[i][j].setNodo(i, j);
        }
    }

     Arista a = Arista(mat[0][0], mat[0][1]);
    // a.imprimirArista();
    for(int i=0; i < num; i++){
         for(int j=0; j < num; j++){
             
            if(j<num-1){
                for(int l=j+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[i][l]);
                   cola_aristas.push(b);
                }
            }


            if(i<num-1){
                for(int l=i+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[l][j]);
                   cola_aristas.push(b);
                }
            }
        }
    }

    while(!cola_aristas.empty()){
        a = cola_aristas.front();
        cola_aristas.pop();
        a.imprimirArista();
        contador++;
    }

    cout<<contador<< endl;
    return 0;
}