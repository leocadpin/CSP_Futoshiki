#include "matdominios.h"

matdominios::matdominios(int tam){

    ancho = tam;
    alto = tam;
    profundo = tam;

    matriz = new int**[tam];
    
    for (int i = 0; i < tam; i++) {
 
        // Allocate memory blocks for
        // rows of each 2D array
        matriz[i] = new int*[tam];
 
        for (int j = 0; j < tam; j++) {
 
            // Allocate memory blocks for
            // columns of each 2D array
            matriz[i][j] = new int[tam];
        }
    }


    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < tam; k++) {
 
                // Assign values to the
                // memory blocks created
                matriz[i][j][k] = 1;
            }
        }
    }

}


matdominios::~matdominios(){

    // Deallocate memory
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < alto; j++) {
            delete[] matriz[i][j];
        }
        delete[] matriz[i];
    }
    delete[] matriz;

}

bool matdominios::enDominio(int fil, int col, int num){
    bool endominio = true;
    num = num - 1;
    if(matriz[fil][col][num] == 0){
        endominio = false;
    }

    return endominio;
}
void matdominios::sacarDominio(int fil, int col, int num){

    num = num - 1;
    matriz[fil][col][num] = 0;
    

}

void matdominios::meterDominio(int fil, int col, int num){

    num = num - 1;
    matriz[fil][col][num] = 1;

}