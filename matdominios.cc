#include "matdominios.h"
#include <iostream>
using namespace std;

//Constructor de la clase dominio, le pasamos el tamaño y crea la matriz de  N x N x N
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

//Sobrecarga delconstructor de la clase dominio, le pasamos el tamaño y crea la matriz de  N x N x N
//tambien le pasamos un valor con el que rellenar la matriz de dominios
matdominios::matdominios(int tam, int valor){

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
                matriz[i][j][k] = valor;
            }
        }
    }

}

//Función para cambiar el tamaño de la matriz
void matdominios::resize_matdominios(int tam){
    // Deallocate memory
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < alto; j++) {
            delete[] matriz[i][j];
        }
        delete[] matriz[i];
    }
    delete[] matriz;
    
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
//Destructor de la clase
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

//Función que comprueba si un numero esta en el dominio
bool matdominios::enDominio(int fil, int col, int num){
    bool endominio = true;
    num = num - 1;
    if(matriz[fil][col][num] == 0){//Si el valor en la tercera dimension de la matriz es 0, el numero no esta en el dominio
        endominio = false;
    }

    return endominio;
}
//Funcion que saca un numero del dominio (lo pone a 0)
void matdominios::sacarDominio(int fil, int col, int num){
    
    num = num - 1;
    matriz[fil][col][num] = 0;
    

}
//Funcion que mete un numero del dominio (lo pone a 1)
void matdominios::meterDominio(int fil, int col, int num){

    num = num - 1;
    matriz[fil][col][num] = 1;

}

//Funcion que enseña por pantalla el dominio
void matdominios::imprimirDominio(){
    int tm = ancho;

    for (int l = 0; l < tm; l++){
        for (int m = 0; m < tm; m++){
            for (int n = 0; n < tm; n++){

          //      cout << "p" << l  <<  m << n << " " << matriz[l][m][n] << " ";
            cout  << matriz[l][m][n];
            }

            cout<< endl;
        }
        cout<< endl;
    }
    
}
