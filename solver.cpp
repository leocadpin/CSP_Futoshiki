#include "solver.h"


Solver::Solver(QObject *parent) : QObject(parent)
{
    backTrackingJumps = 0;
}

void Solver::ejecutarBT(Tablero *tablero)
{
    int tam = tablero -> getSize(); // tamaño del tablero
    int fil=tam-1, col=tam-1;

    bt_futoshiki(tablero, fil, col, tam);
    std::cout << this->backTrackingJumps <<  std::endl;
    /*
    for(int i = 0; i<tam; i++){
        for(int j = 0; j<tam; j++ ){

            // Si la posicion ya esta ocupada la saltamos
            elem = tablero->getElement(i,j);
            //Nos quedamos con la primera posicion sin ser rellenada
            if(elem == 0){
                fil = i;
                col = j;

                i=tam+1;
                j=tam+1;
            }

        }
    }
    */
    //probamos con elementos de 1 a n



}

bool Solver::bt_futoshiki(Tablero *tablero, int fil, int col, int tam){
    //caso base: si hemos rellenado todas las casillas
    backTrackingJumps ++;
    //std::cout << this->backTrackingJumps <<  std::endl;
    bool res=false;

    //REVISAMOS SI LA CASILLA ESTA OCUPADA O NO PARA VER SI TENDREMOS QUE MODIFICARLA
    int elem = tablero->getCasilla(fil,col);

    //
    if(elem == 0){
        for(int l=1; l<=tam ; l++){

            if(factible(tablero, fil, col, tam, l)){
                tablero->setCasilla(fil, col, l);

                //ESTE ES NUESTRO CASO BASE: CUANDO LLEGAMOS A COMPLETAR EL
                //TABLERO TENDREMOS LA SOLUCION
                if(tablero->tableroCompleto() ){

                    return res=true;
                }

                //LLAMADAS A LA RECURSIVIDAD
                if(col <=0){
                    res =   bt_futoshiki(tablero, fil-1,tam-1,tam);
                }
                else{
                    res=bt_futoshiki(tablero, fil,col-1,tam);
                }

                //IMPORTANTE INDICAR QUE TERMINEMOS LA FUNCION SI YA HEMOS ENCONTRADO LA SOLUCION
                if(res==true){
                    return true;
                }

                //SI HEMOS PROBADO TODOS LOS CASOS DEVOLVEMOS FALSE Y LLENAMOS CON 0 LA CASILLA
                if(l==tam){
                    tablero->setCasilla(fil, col, 0);
                    return false;
                }
            }
            else{
                if(l==tam){
                    return false;
                }
            }
        }
    }
    else{
        //AUNQUE NO LO PODEMOS MODIFICAR, DEBEMOS COMPROBAR SI LA
        // CUMPLE RESTRICCIONES PARA EVITAR ERRORES EN LA SOLUCION YSI SE DA EL CASO DESCARTAR
        if(factible(tablero, fil, col, tam, elem)){
            tablero->setCasilla(fil, col, elem);
            if(tablero->tableroCompleto()){
                return true;
            }
            if(col <=0){
            res=    bt_futoshiki(tablero, fil-1,tam-1,tam);
            }
            else{
            res=    bt_futoshiki(tablero, fil,col-1,tam);
            }
        }
        else{
            tablero->setCasilla(fil, col, elem);

            return false;
        }

    }

    if(res==true){
        return true;
    }

}

bool Solver::factible(Tablero *tablero, int fil, int col, int tam, int elem){

   bool fac = true;

   //SE REPITE?
   tablero->setCasilla(fil, col, elem);

   if((tablero->estaEnFila(fil,col)) || (tablero->estaEnCol(fil,col))  ){


       fac = false;
   }
   //CONDICIONES BINARIAS
   ///*
   if(fil < tam-1){
       if(tablero->comprobarBinariaSiguienteVertical(fil, col)){

           fac = false;
       }
   }
   if(col < tam-1){
       if(tablero->comprobarBinariaSiguienteHorizontal(fil, col)){

            fac = false;

       }
   }//*/


    tablero->setCasilla(fil, col, 0);
   return fac;
}

void Solver::ejecutarAC3(Tablero *tablero)
{
    std::cout<<"AC3: "<<"Esta función no está todavía implementada."<<std::endl;

    return;
}

void Solver::ejecutarFC(Tablero *tablero)
{
    std::cout<<"Forward Checking: "<<"Esta función no está todavía implementada."<<std::endl;

    return;
}

