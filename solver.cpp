#include "solver.h"
#include "arista.cc"
#include "matdominios.cc"
#include <deque>
#include<math.h>
 matdominios dominio = matdominios(1);

Solver::Solver(QObject *parent) : QObject(parent)
{
    backTrackingJumps = 0;
}



void Solver::ejecutarBT(Tablero *tablero)
{
    int tam = tablero -> getSize(); // tamaño del tablero
    int fil=tam-1, col=tam-1;
    dominio.resize_matdominios(tam);
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

            if(dominio.enDominio(fil, col, l )){
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
            else {
                if(l==tam){
                    tablero->setCasilla(fil, col, 0);

                }
               continue;
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
    deque< Arista > cola_aristas;
    deque< Arista > cola_descartes;
    dominio.resize_matdominios(num);

    int debug=0;

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
                   cola_aristas.push_back(b);
                   b = Arista( mat[i][l], mat[i][j]);
                   cola_aristas.push_back(b);
                }
            }


            if(i<num-1){
                for(int l=i+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[l][j]);
                   cola_aristas.push_back(b);
                   b = Arista( mat[l][j], mat[i][j]);
                   cola_aristas.push_back(b);
                }
            }
        }
    }

// ESQUEMA AC3 
    while(!cola_aristas.empty()){
        
        Arista arya = cola_aristas.front();
        arya.imprimirArista();

        arya_n1 = arya.get_n1();
        arya_n2 = arya.get_n2();
//        if((3 == arya_n1.get_x()) and (0 == arya_n1.get_y()) ){
//            if((2 == arya_n2.get_x()) and (0 == arya_n2.get_y()) ){
//            break;
//            }
//        }
        cola_descartes.push_back(arya);
        cola_aristas.pop_front();

        centinela_dominio_dk = 0;
        cambio=false;
        for(int i=1; i <=num; i++ ){

            if(dominio.enDominio(arya_n1.get_x(), arya_n1.get_y(), i )){
                if(!consistente(tablero, i, arya_n1.get_x(), arya_n1.get_y(), arya_n2.get_x(), arya_n2.get_y())){
                    
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
                cola_descartes.pop_front();

                if((reserva_n2.get_x() == arya_n1.get_x()) and (reserva_n2.get_y() == arya_n1.get_y()) ){
                    cola_aristas.push_back(reserva);
                }
                else{
                    cola_descartes.push_back(reserva);
                }
                aux_centinela++; 
            }
                
        }

    dominio.imprimirDominio();
    debug++;
    std::cout << debug << endl;
    }

    dominio.imprimirDominio();
    int fil=num-1;
    int col=num-1;
    bt_futoshiki(tablero, fil, col, num);
    return;
}

bool Solver::consistente(Tablero *tablero, int num_Vk, int x_n1, int y_n1, int x_n2, int y_n2){
    bool resultado=false;
    float dist ;
    int dist2, arriba=0, abajo=0, derecha=0,izquierda=0, tamta = tablero->getSize();
    dist =sqrt(((x_n2-x_n1)*(x_n2-x_n1)) + ((y_n2-y_n1)*(y_n2-y_n1)));
    for(int contando=1; contando <= tamta ; contando++  ){
        if(dominio.enDominio(x_n2, y_n2, contando)){
            if(num_Vk==contando){
                // if(){
                continue;
                // }
            }


            if(dist<=1.0){
                if(x_n1 < tamta-1){

                    abajo = tablero->getElement(2*x_n1+1, y_n1*2);

                }
                if(x_n1 > 0){

                    arriba = tablero->getElement(2*x_n1-1, y_n1*2);

                }
                if(y_n1 < tamta-1){

                    derecha = tablero->getElement(x_n1*2, 2*y_n1+1);

                }
                if(y_n1 > 0){

                    izquierda = tablero->getElement(2*x_n1, 2*y_n1-1);

                }

                if(x_n1 == x_n2){
                    dist2 = y_n2 - y_n1;
                    if(dist2 == 1){
                        if(derecha==1){
                            if(num_Vk < contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(derecha== -1){
                            if(num_Vk > contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(derecha== 0){
                            
                                resultado=true;
                            break;
                        }
                    }
                    else{ // dist==-1
                        if(izquierda==1){
                            if(num_Vk > contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(izquierda== -1){
                            if(num_Vk < contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(izquierda== 0){
                            
                                resultado=true;
                            break;
                        }
                    }
                }

                if(y_n1 == y_n2){
                    dist2 = x_n2 - x_n1;
                    if(dist2 == -1){
                        if(arriba==1){
                            if(num_Vk > contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(arriba== -1){
                            if(num_Vk < contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(arriba== 0){
                            
                                resultado=true;
                                break;
                            
                        }
                    }
                    else{ // dist==-1
                        if(abajo==1){
                            if(num_Vk < contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(abajo == -1){
                            if(num_Vk > contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(abajo== 0){
                            
                                resultado=true;
                                break;
                        }
                    }
                }

            }
            else{
                resultado=true;
                break;
            }
            
        }
        else{
            continue;
        }
    }   
    return resultado;
}

void Solver::ejecutarFC(Tablero *tablero)
{
    std::cout<<"Forward Checking: "<<"Esta función no está todavía implementada."<<std::endl;

    return;
}

