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
    backTrackingJumps = 0;

    bt_futoshiki(tablero, 0, 0, tam);
    std::cout << this->backTrackingJumps <<  std::endl;   


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
                        if(col ==tam-1){
                            res =   bt_futoshiki(tablero, fil+1,0,tam);
                        }
                        else{
                            res=bt_futoshiki(tablero, fil,col+1,tam);
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
            if(col ==tam-1){
            res=    bt_futoshiki(tablero, fil+1,0,tam);
            }
            else{
            res=    bt_futoshiki(tablero, fil,col+1,tam);
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
   if(fil >0 ){
       if(tablero->comprobarBinariaSiguienteVertical(fil-1, col)){

           fac = false;
       }
   }
   if(col > 0){
       if(tablero->comprobarBinariaSiguienteHorizontal(fil, col-1)){

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




    }


    int fil=num-1;
    int col=num-1;
    backTrackingJumps = 0;

    //bt_futoshiki(tablero, 0, 0, num);

    dominio.imprimirDominio();
    std::cout << this->backTrackingJumps <<  std::endl;
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


    int tam = tablero -> getSize(); // tamaño del tablero
    int fil=tam-1, col=tam-1;
    dominio.resize_matdominios(tam);

    int elem = 0;
    backTrackingJumps = 0;
    ejecutarAC3(tablero);
    for(int i=0; i < tam; i++){
        for(int j=0; j < tam; j++){
            
            elem = tablero->getCasilla(i,j);
            if(elem != 0){
                for(int k=1; k<=tam; k++){
                    if(k-1 != i){
                        dominio.sacarDominio(k-1, j, elem);
                    }
                    if(k-1 != j){
                        dominio.sacarDominio(i, k-1, elem);
                    }

                    if(k==elem){
                        continue;
                    }
                    dominio.sacarDominio(i, j, k);

                }

            }
      }
    }

    fc_futoshiki(tablero, 0, 0, tam);

    for(int i = 0; i<tam; i++){
        for(int j = 0; j<tam; j++){
            for(int k = 1; k<=tam; k++){
                if(dominio.enDominio(i,j,k)){
                    tablero->setCasilla(i,j,k);
                
                }
            }
        }
    }
    std::cout << this->backTrackingJumps <<  std::endl;
    return;
}

bool Solver::fc_futoshiki(Tablero *tablero, int fil, int col, int tam){
    int xi;
    matdominios podado = matdominios(tam, 0);
    backTrackingJumps ++;
    for(int i = 1; i<= tam; i++){
        if(dominio.enDominio(fil, col, i)){
           xi= i;
           instanciar(podado, xi, fil, col, tam);
           if(fil == tam-1 and col==tam-1){
               return true;
           }
           else{

               if(forward(tablero, fil, col, tam, xi, podado)){
                    if(col==tam-1){

                        if(fc_futoshiki(tablero, fil+1, 0, tam)){
                            return true;
                        }
                    }
                    else{
                        if(fc_futoshiki(tablero, fil, col+1, tam)){
                            return true;
                        }
                    }    
               }

               restaura(podado, fil, col, tam);


           }
           if(i==tam){
               return false;
           }


        }
        else{
            
            continue;
        }
    }
    return false;
}

//Funcion que realiza la comprobacion hacia adelante
bool Solver::forward(Tablero *tablero, int fil, int col, int tam, int valor, matdominios &podado){
    // Comprobamos los valores en el dominio de las filas siguientes

    for(int fila_forward = fil+1; fila_forward < tam; fila_forward++){
        bool vacio = true;
        
        for(int k=1; k <= tam; k++){
            //Primero comprobamos hacia adelante con las filas
            if(dominio.enDominio(fila_forward, col, k)){
                
                
                


                if(consistente_fc(tablero, valor, fil, col, k, fila_forward, col)){
                    vacio = false;
                }
                else{
                    // el valor k no es consistente con la asignacion puesta 
                    if(tablero->getCasilla(fila_forward, col) == 0){
                        dominio.sacarDominio(fila_forward, col, k);
                        podado.meterDominio(fila_forward,col, k);
                    }
               }



            }
            //Si el valor no esta en el dominio de la siguiente fila


            
        }
        if(vacio){
            return false;
        }


    }
    
    for(int columna_forward = col+1; columna_forward<tam; columna_forward ++){
        bool vacio2 = true;
        for (int k2 = 1; k2 <= tam; k2++)
        {
            
            if(dominio.enDominio(fil, columna_forward, k2)){
                if(consistente_fc(tablero, valor, fil, col, k2, fil, columna_forward)){
                    vacio2 = false;
                }
                else{
                    // el valor k no es consistente con la asignacion puesta 
                    if(tablero->getCasilla(fil, columna_forward) == 0){
                        dominio.sacarDominio(fil, columna_forward, k2);
                        podado.meterDominio(fil,columna_forward, k2);
                    }
                }



            }

            //Si el valor no esta en el dominio de la siguiente fila
            


        }
        if(vacio2){
            return false;
        }
        


    }

    return true;
}

bool Solver::consistente_fc(Tablero *tablero, int num_Vk, int x_n1, int y_n1, int contando, int x_n2, int y_n2){
    bool resultado=false;
    float dist ;
    int dist2, arriba=0, abajo=0, derecha=0,izquierda=0, tamta = tablero->getSize();
    dist =sqrt(((x_n2-x_n1)*(x_n2-x_n1)) + ((y_n2-y_n1)*(y_n2-y_n1)));
    
        
            if(num_Vk==contando){
                
                return false;
                
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
                                
                            }
                        }
                        if(derecha== -1){
                            if(num_Vk > contando){
                                resultado=true;
                                
                            }
                        }
                        if(derecha== 0){
                            
                                resultado=true;
                            
                        }
                    }
                    else{ // dist==-1
                        if(izquierda==1){
                            if(num_Vk > contando){
                                resultado=true;
                                
                            }
                        }
                        if(izquierda== -1){
                            if(num_Vk < contando){
                                resultado=true;
                                
                            }
                        }
                        if(izquierda== 0){
                            
                                resultado=true;

                        }
                    }
                }

                if(y_n1 == y_n2){
                    dist2 = x_n2 - x_n1;
                    if(dist2 == -1){
                        if(arriba==1){
                            if(num_Vk > contando){
                                resultado=true;
                                
                            }
                        }
                        if(arriba== -1){
                            if(num_Vk < contando){
                                resultado=true;
                                
                            }
                        }
                        if(arriba== 0){
                            
                                resultado=true;
                                
                            
                        }
                    }
                    else{ // dist==-1
                        if(abajo==1){
                            if(num_Vk < contando){
                                resultado=true;
                                
                            }
                        }
                        if(abajo == -1){
                            if(num_Vk > contando){
                                resultado=true;
                                
                            }
                        }
                        if(abajo== 0){
                            
                                resultado=true;
                                
                        }
                    }
                }

            }
            else{
                resultado=true;
                
            }
            
        
        
        
        
    
    return resultado;
}

void Solver::instanciar(matdominios &podado, int xi, int fil, int col, int tam){

    for(int k=1; k<=tam; k++){
        if(k-1 != fil){
            if(dominio.enDominio(k-1, col, xi)){
                dominio.sacarDominio(k-1, col, xi);
                podado.meterDominio(k-1, col, xi);
            }
        }
        if(k-1 != col){
            if(dominio.enDominio(fil, k-1, xi)){
                dominio.sacarDominio(fil, k-1, xi);
                podado.meterDominio(fil, k-1, xi);
            }
        }

        if(k==xi){
            continue;
        }
        if(dominio.enDominio(fil, col, k)){
            dominio.sacarDominio(fil, col, k);
            podado.meterDominio(fil, col, k);
        }
    }
//    std::cout << "PODAAAAA DEL " << xi << " en las casillas " << fil << " " << col << endl;
//    podado.imprimirDominio();

}

void Solver::restaura(matdominios &podado, int fil, int col, int tam){
 
//    std::cout << "PODAAAAAaaaaaaaa en las casillas " << fil << " " << col << endl;
//    podado.imprimirDominio();
    for(int i = fil; i<tam; i++){
        for(int j = col; j < tam; j++){
            for(int k = 1 ; k <= tam; k++){
                if(podado.enDominio(i,j, k)){
                    podado.sacarDominio(i, j, k);
                    dominio.meterDominio(i, j, k);
                }
            }
        }
    }

//    std::cout << "RESTAURACION en las casillas " << fil << " " << col << endl;
//    podado.imprimirDominio();

}
