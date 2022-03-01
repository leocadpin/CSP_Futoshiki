#include "solver.h"
#include "arista.cc"
#include "matdominios.cc"
#include <queue>



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
    //std::cout<<"AC3: "<<"Esta función no está todavía implementada."<<std::endl;
    int num = tablero -> getSize();
    int elem, centinela_dominio_dk = 0, aux_centinela=0, tam_kiwi;
    //int contador=0;
    //Arista arya, reserva;
    Nodo mat[num][num];
    Nodo arya_n1, arya_n2, reserva_n1, reserva_n2;
    bool cambio = false;
    queue< Arista > cola_aristas;
    queue< Arista > cola_descartes;
    matdominios dominio = matdominios(num);



    for(int i=0; i < num; i++){
        for(int j=0; j < num; j++){
            mat[i][j].setNodo(i, j);
            elem = tablero->getCasilla(i,j);
            if(elem != 0){
                for(int k=1; k<=num; k++){
                    if(k==elem){
                        continue;
                    }
                    dominio.sacarDominio(i, j, k);
                }
            }
      }
    }

    dominio.imprimirDominio();


    for(int i=0; i < num; i++){
         for(int j=0; j < num; j++){
             
            if(j<num-1){
                for(int l=j+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[i][l]);
                   cola_aristas.push(b);
                   b = Arista( mat[i][l], mat[i][j]);
                   cola_aristas.push(b);
                }
            }


            if(i<num-1){
                for(int l=i+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[l][j]);
                   cola_aristas.push(b);
                   b = Arista( mat[j][j], mat[i][j]);
                   cola_aristas.push(b);
                }
            }
        }
    }
// ESQUEMA AC3 
    while(!cola_aristas.empty()){
        
        Arista arya = cola_aristas.front();
        arya_n1 = arya.get_n1();
        arya_n2 = arya.get_n2();
        cola_descartes.push(arya);
        cola_aristas.pop();

        centinela_dominio_dk = 0;

        for(int i=1; i <=num; i++ ){
         
            if(dominio.enDominio(arya_n1.get_x(), arya_n1.get_y(), i )){
                if(!consistente(tablero, i, arya_n2.get_x(), arya_n2.get_y())){
                    
                    dominio.sacarDominio(arya_n1.get_x(), arya_n1.get_y(), i);
                    centinela_dominio_dk++ ;
                    cambio =true;

                }
            }
            else{//SI i no esta en el dominio de  Vk 
                centinela_dominio_dk++ ;
                continue;

            }
        }

        if(centinela_dominio_dk == num){
            std::cout << "Salimos sin solucion" << endl;
            return;
        }
        
        if(cambio){

            tam_kiwi = cola_descartes.size();
            aux_centinela=0;
            while(aux_centinela < tam_kiwi){
                
                // if((reserva.n2.get_x() == ari_centinela.n2.get_x()) and (reserva.n2.get_y() == ari_centinela.n2.get_y()) ){
                //     if((reserva.n1.get_x() == ari_centinela.n1.get_x()) and (reserva.n1.get_y() == ari_centinela.n1.get_y())){
                //         sentinela= true;
                //     }
                // }


                Arista reserva = cola_descartes.front();
                reserva_n1 = reserva.get_n1();
                reserva_n2 = reserva.get_n2();
                cola_descartes.pop();

                if((reserva_n2.get_x() == arya_n1.get_x()) and (reserva_n2.get_y() == arya_n1.get_y()) ){
                    cola_aristas.push(reserva);
                }
                else{
                    cola_descartes.push(reserva);
                }
                aux_centinela++; 
            }
                
        }
        
    }

    
    return;
}

bool Solver::consistente(Tablero *tablero, int num_Vk, int x, int y){
    bool resultado=false;
    for(int contando=0; contando<tablero->getSize(); contando++  ){
        // if(i==dominio[]){

        // }
    }   
    resultado;
}

void Solver::ejecutarFC(Tablero *tablero)
{
    std::cout<<"Forward Checking: "<<"Esta función no está todavía implementada."<<std::endl;

    return;
}

