#include "solver.h"
#include "arista.cc"
#include "matdominios.cc"
#include <deque>
#include<math.h>

//Esta variable global representa el dominio del tablero
//como por defecto no sabemos su tamaño, primero la definimos con tamaño 1x1x1
matdominios dominio = matdominios(1);


Solver::Solver(QObject *parent) : QObject(parent)
{
    backTrackingJumps = 0;
}


/*Funcion para ejecutar backtracking y llamar a la rescursividad



*/
void Solver::ejecutarBT(Tablero *tablero)
{
    int tam = tablero -> getSize(); // tamaño del tablero
    //int fil=tam-1, col=tam-1;
    dominio.resize_matdominios(tam); // Le damos el tamaño necesario al dominio
    //Ponemos la variable backtrackingjumps a 0 para contabilizar cuantas llamadas ha hecho la funcion
    backTrackingJumps = 0;              
    unsigned t0, t1;

        t0 = clock();
    //Llamamos a la función recursiva que sigue el esquema backtracking. Empezaremos desde la casilla 0,0
    bt_futoshiki(tablero, 0, 0, tam);
    //Medimos tiempo
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    std::cout << "El tiempo de backtracking para " << tam << "x" << tam << " han sido: " << time <<  std::endl;
    //Medimos los pasos
    std::cout << "Los pasos de backtracking para " << tam << "x" << tam << " han sido: " << this->backTrackingJumps <<  std::endl;


}

bool Solver::bt_futoshiki(Tablero *tablero, int fil, int col, int tam){
    
    backTrackingJumps ++; //Añadimos una llamada a la cuenta
    bool res=false;    //Por defecto el resultado es falso

    //CASO BASE, SI HEMOS RELLENADO TODO EL TABLERO ASUMIMOS QUE EL RESULTADO ES CORRECTO
    if(tablero->tableroCompleto() ){
        return true;
    }


    

    //REVISAMOS SI LA CASILLA ESTA OCUPADA O NO PARA VER SI TENDREMOS QUE MODIFICARLA
    int elem = tablero->getCasilla(fil,col);

    //SI EL VALOR EN LA CASILLA ES 0, PROCEDEMOS A HACER EL ANALISIS
    if(elem == 0){
        for(int l=1; l<=tam ; l++){//Probamos con los numeros que pueden ir en la casilla (1 a N)
            
            //Primero comprobamos si el numero esta en el dominio. Esto será relevante tras AC3
            // en el esquema backtracking, todos los dominios estan disponibles
            if(dominio.enDominio(fil, col, l )){
                //Ahora comprobamos si el poner el numero en la casilla es factible
                // la funcion factible se encuenr desglosasda mas adelante
                if(factible(tablero, fil, col, tam, l)){
                    tablero->setCasilla(fil, col, l); //Como es factible, seteamos la casilla

                

                    //LLAMADAS A LA RECURSIVIDAD
                    if(col ==tam-1){//Si hemos llegado a la ultima columna, iremos a la siguiente fila y la columna 0
                        res =   bt_futoshiki(tablero, fil+1,0,tam);
                    }
                    else{//Si no, nos mantenemos en la fila pero vaos a la siguiente columna
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
                else{//Si no es factible poner el numero en la casilla no hacemos nada, y si hemos llegado al ultimo
                    //numero, devolvemos falso
                    if(l==tam){
                        return false;
                    }
                }

            }
            else{//Si nuestro numero está fuera del dominio, pasamos al siguiente
               if(l==tam){
                    tablero->setCasilla(fil, col, 0);

                }
               continue;
            }
        }
    }
    else{//EN ESTE CASO LA CASILLA ES !=0
        //AUNQUE NO LO PODEMOS MODIFICAR, DEBEMOS COMPROBAR SI LA
        // CUMPLE RESTRICCIONES PARA EVITAR ERRORES EN LA SOLUCION YSI SE DA EL CASO DESCARTAR
        
        //Si el numero predefinido es factible, lo dejamos y seguimos con la recursividad
        if(factible(tablero, fil, col, tam, elem)){
            tablero->setCasilla(fil, col, elem);
           
            if(col ==tam-1){
            res=    bt_futoshiki(tablero, fil+1,0,tam);
            }
            else{
            res=    bt_futoshiki(tablero, fil,col+1,tam);
            }
        }
        else{//Si no, devolvemos falso, pues no podemos probar mas numeros para esta casilla
            
            tablero->setCasilla(fil, col, elem);
            return false;
        }

    }

    if(res==true){
        return true;
    }

}


//FUNCION QUE COMPRUEBA SI PONER UN NUMERO EN UNA POSICION ES FACTIBLE
bool Solver::factible(Tablero *tablero, int fil, int col, int tam, int elem){

   bool fac = true;

   //Metemos el numero en la casilla
   tablero->setCasilla(fil, col, elem);

    //Si el numero ya esta en la fila o en la columna, no sera factible
    if((tablero->estaEnFila(fil,col)) || (tablero->estaEnCol(fil,col))  ){
        fac = false;
    }

   //CONDICIONES BINARIAS: comprobaremos las rstricciones con respecto a las casillas de arriba y a la izquierda
   // porque, al recorrer el tablero hacia la derecha y hacia abajo,si comparasemos con los numeros de nuestraderecha y abajo
   //siempre veriamos 0, que no nos llevaria a la solucion adecuada
   
   if(fil >0 ){//Si no estamos en la columna 0
        //Si el numero no cumple la restriccion de casilla con la posicion que esta encima de ella, no será factible
       if(tablero->comprobarBinariaSiguienteVertical(fil-1, col)){
           fac = false;
       }
   }
   if(col > 0){
       //Si el numero no cumple la restriccion de casilla con la posicion que esta a la izquierda de ella, no será factible
        if(tablero->comprobarBinariaSiguienteHorizontal(fil, col-1)){
            fac = false;
        }    
    }

    //Seteamos la casilla a 0, ya que el objetivo de esta funcion es unicamente decir si 
    // el numero es factible o no, pero su uso no debe modificar parametros del tablero con respecto a otras funciones
    tablero->setCasilla(fil, col, 0);
  
   return fac;
}

//FUNCION DONDE SE RALIZARÁ LA EJECUCION DEL ALGORITMO AC3
void Solver::ejecutarAC3(Tablero *tablero)
{
    unsigned t0, t1;

    t0 = clock();
    //Declaramos una serie de variables que utilizaremos
    int num = tablero -> getSize(); 
    int elem, centinela_dominio_dk = 0, aux_centinela=0, tam_kiwi;
    
    //Matriz de nodos que representa el tablero
    Nodo mat[num][num]; 
    
    //Nodos que se analizarán durante el algoritmo
    Nodo arya_n1, arya_n2, reserva_n1, reserva_n2; //->Nodos de la arista principal y nodos de la arista descartada
    
    //Variable que controla si hemos cambiado el dominio de un nodo
    bool cambio = false;

    //Colas de doble orientacion que usaremos para almacenar las aristas del tablero y las aristas
    // que vayamos a descartar, para cuando haga falta volver a analizaralas

    deque< Arista > cola_aristas;
    deque< Arista > cola_descartes;

    //Le damos al dominio el tamaño del tablero
    dominio.resize_matdominios(num);


    //Este primer bucle doble elimina de los dominios de las casillas que se encuentran predefinidas en el tablero 
    // todos los numeros menos el predefinido. Tambien saca el numero predefinido de los dominios de la misma fila o columna
    for(int i=0; i < num; i++){
        for(int j=0; j < num; j++){
            mat[i][j].setNodo(i, j);
            elem = tablero->getCasilla(i,j);
            if(elem != 0){
                for(int k=1; k<=num; k++){
                    // if(k-1 != i){
                    //     dominio.sacarDominio(k-1, j, elem);
                    // }
                    // if(k-1 != j){
                    //     dominio.sacarDominio(i, k-1, elem);
                    // }
                    if(k==elem){
                        continue;
                    }
                    dominio.sacarDominio(i, j, k);
                }
            }
      }
    }
    
    

    //Ahora creamos las aristas del tablero
    //Recorremos la matriz de nodos 
    for(int i=0; i < num; i++){
         for(int j=0; j < num; j++){
             
            //Si no estamos en la ultima columna, creamos aristas del nodo actual con todos
            //los nodos de la misma fila a partir de la columna de adelante (no lo hacemos con todos
            //los de la misma fila porque si no estariamos creando aristas repetidas)
            if(j<num-1){
                for(int l=j+1; l<num; l++){
                   Arista b = Arista( mat[i][j], mat[i][l]);
                   cola_aristas.push_back(b);
                   b = Arista( mat[i][l], mat[i][j]);
                   cola_aristas.push_back(b);
                }
            }

            //Aqui hacemos lo mismo, pero con las filas
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

// ESQUEMA AC3: repetiremos el algoritmo hasta que no queden aristas por revisar
    while(!cola_aristas.empty()){
        //Tomamos la arista que haya al frente de la cola, y los nodos de esta
        Arista arya = cola_aristas.front();
        arya_n1 = arya.get_n1();
        arya_n2 = arya.get_n2();

        cola_descartes.push_back(arya); //Metemos la arista en la cola de descartes por si hace falta revisarla luego

        cola_aristas.pop_front();   //Sacamos la arista de la cola principal

        centinela_dominio_dk = 0;
        cambio=false;

        //Para cada numero en el dominio del nodo n1 de nuestra arista hacemos lo siguiente
        for(int i=1; i <=num; i++ ){
            if(dominio.enDominio(arya_n1.get_x(), arya_n1.get_y(), i )){
                //Comprobamos si el valor es consistente con el nodo n2 de la arista
                if(!consistente(tablero, i, arya_n1.get_x(), arya_n1.get_y(), arya_n2.get_x(), arya_n2.get_y())){
                    
                    //Si no es consistente, sacamos el valor del dominio en la posicion del nodo n1
                    dominio.sacarDominio(arya_n1.get_x(), arya_n1.get_y(), i);
                    centinela_dominio_dk++ ;
                    cambio =true; //Se ha dado un cambio en el dominio

                }
            }
            else{//Si el valor no esta en el dominio de  Vk, sumamos  +1 al centinela
                centinela_dominio_dk++ ; //Nos sirve para ver si el dominio quedará vacio
                continue;
            }
        }
        //Si el dominio queda vacio, salimos sin solucion
        if(centinela_dominio_dk == num){
            std::cout << "Salimos sin solucion" << endl;
            return;
        }
        
        //Si hemos realizado algun cambio en el dominio de una posicion, debemos revisar
        // todas las aristas que apuntaban a ese nodo, para ello acudimos a la cola de descartes
        if(cambio){

            tam_kiwi = cola_descartes.size();
            aux_centinela=0;

            //Revisamos todas las aristas de la cola descartes, pues a priori no sabemos cuales apuntan al nodo
            // n1 de nuestra arista
            while(aux_centinela < tam_kiwi){
                
                //Tomamos la arista de al frente de la cola de descartes y los nodos
                Arista reserva = cola_descartes.front(); 
                reserva_n1 = reserva.get_n1();
                reserva_n2 = reserva.get_n2();
                cola_descartes.pop_front();

                //Si las coordenadas de n2 de la cola descartes coinciden con las de n1 de la cola principal,
                // siginifica que la arista descartada apunta a n1, asi que volvemos a meter la arista descartada
                // en la principal
                if((reserva_n2.get_x() == arya_n1.get_x()) and (reserva_n2.get_y() == arya_n1.get_y()) ){                    
                    cola_aristas.push_back(reserva);
                }
                else{//Si no apunta a n1, volvemos a meter la arista en la cola de reservas
                    cola_descartes.push_back(reserva);
                }
                aux_centinela++; //Vamos aumentando esta varible hasta que se recorre toda la cola de descartes
            }
                
        }




    }    
    backTrackingJumps = 0;
    //Ahora que hemos acotado los dominios, usamos el backtracking para resolver el problema
    bt_futoshiki(tablero, 0, 0, num);
    //dominio.imprimirDominio();
    //Medimos tiempo
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    std::cout << "El tiempo de AC3 para " << num << "x" << num << " han sido: " << time <<  std::endl;
    //Medimos los pasos
    std::cout << "Los pasos de AC3 para " << num << "x" << num << " han sido: " << this->backTrackingJumps <<  std::endl;

    return;
}


//FUNCION PARA COMPROBAR SI UN VALOR ES CONSISTENTE CON LA ARISTA A LA QUE APUNTA
bool Solver::consistente(Tablero *tablero, int num_Vk, int x_n1, int y_n1, int x_n2, int y_n2){
    bool resultado=false;
    //Variable que nos dara la distancia entre 2 nodos
    float dist;

    int dist2, arriba=0, abajo=0, derecha=0,izquierda=0, tamta = tablero->getSize();
    dist =sqrt(((x_n2-x_n1)*(x_n2-x_n1)) + ((y_n2-y_n1)*(y_n2-y_n1)));

    //Hacemos las comprobaciones para cada numero del dominio de n2
    for(int contando=1; contando <= tamta ; contando++  ){
        if(dominio.enDominio(x_n2, y_n2, contando)){

            //Si el valor de n1 es igual al de n2, el resultdo seguira siendo falso, 
            // y el valor de n2 no hace consistente al de n1
            if(num_Vk==contando){                
                continue;                
            }

            //Realizaremos las comprobaciones de distancia si el numero esta a una distancia menor a 1
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

                //Si los valores se encuentran en la misma fila
                if(x_n1 == x_n2){
                    //Calculamos este valor restando las coordenadas en Y, para saber si el valor de n2
                    //esta a la izquierda o a la derecha
                    dist2 = y_n2 - y_n1;

                    //Si el resultado es 1, el val_n2 esta a la derecha
                    if(dist2 == 1){
//                        //Tomamos el valor que esta a la derecha, dependiendo de si es 1,-1 o 0.
//                        if(y_n1 < tamta-1){
//                            derecha = tablero->getElement(x_n1*2, 2*y_n1+1);
//                        }
                        if(derecha==1){
                            //Si el elemento es 1, representa un "<", por lo que para cumplir la restriccion
                            //val_n1 debe ser menor que val_n2
                            if(num_Vk < contando){
                                resultado=true;
                                break;
                            }
                        }
                        if(derecha== -1){
                            //Si el elemento es -1, representa un ">", por lo que para cumplir la restriccion
                            //val_n1 debe ser mayor que val_n2
                            if(num_Vk > contando){
                                resultado=true;
                                break;
                            }
                        }                        
                        if(derecha== 0){
                            //Si el elemento es 0, no hay restriccion de mayor o menor                            
                            resultado=true;
                            break;
                        }
                    }
                    else{ //Si el resultado es -1, el val_n2 esta a la izquierda
//                        if(y_n1 > 0){//vemos si no estamos en la columna 0
//                            izquierda = tablero->getElement(2*x_n1, 2*y_n1-1);
//                        }

                        //Seguimos el mismo tipo de esquema de comprobacion que hemos usado mas arriba
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
                        if(izquierda == 0){                            
                            resultado=true;
                            break;
                        }
                    }
                }

                //En este caso los nodos se encuentran en la misma columna, y seguimos un esquema análogo 
                // a las comprobaciones de las filas
                if(y_n1 == y_n2){
                    dist2 = x_n2 - x_n1;
                    if(dist2 == -1){
//                        if(x_n1 > 0){
//                            arriba = tablero->getElement(2*x_n1-1, y_n1*2);
//                        }
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
//                        if(x_n1 < tamta-1){
//                            abajo = tablero->getElement(2*x_n1+1, y_n1*2);
//                        }
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
            else{//Si la distancia es mayor a 1 y no son iguales, los valores son consistentes
                resultado=true;
                break;
            }
            
        }
        else{//Si no esta en el dominio seguimos comprobando
            continue;
        }
    }   
    return resultado;
}


//FUNCION PARA EJECUTAR EL FORWARD CHECKING
void Solver::ejecutarFC(Tablero *tablero){
    int tam = tablero -> getSize(); // tamaño del tablero
    unsigned t0, t1;


    //Le damos al dominio el tamaño del tablero
    dominio.resize_matdominios(tam);

    int elem = 0;
    
    
    t0 = clock();
    //Este primer bucle doble elimina de los dominios de las casillas que se encuentran predefinidas en el tablero 
    // todos los numeros menos el predefinido. Tambien saca el numero predefinido de los dominios de la misma fila o columna
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
    //ejecutarAC3(tablero); //Esto es para cuando probamos AC3 Y FORWARD
    //Llamamos a la funcion recursiva de forward checking, empezaremos en la posicion 0,0
    backTrackingJumps = 0;
    fc_futoshiki(tablero, 0, 0, tam);
    //dominio.imprimirDominio();
    t1 = clock();
    //Con este bucle rellenamos el tablero, una vez obtenida la solucion para los dominios con Forward Checking
    for(int i = 0; i<tam; i++){
        for(int j = 0; j<tam; j++){
            for(int k = 1; k<=tam; k++){
                if(dominio.enDominio(i,j,k)){
                    tablero->setCasilla(i,j,k);                
                }
            }
        }
    }
    //Medimos tiempo
    
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    std::cout << "El tiempo de forward chicken para " << tam << "x" << tam << " han sido: " << time <<  std::endl;
    //Medimos los pasos
    std::cout << "Los pasos de forward checking para " << tam << "x" << tam << " han sido: " << this->backTrackingJumps <<  std::endl;

    return;
}

//FUNCION RECURSIVA DE FORWARD CHECKING
bool Solver::fc_futoshiki(Tablero *tablero, int fil, int col, int tam){

    int xi;
    //Definimos una matriz de podados para tener en cuenta los valores que saquemos del dominio,
    // por si los debemos restaurar
    matdominios podado = matdominios(tam, 0);
    backTrackingJumps ++;
    
    //Realizamos las comprobaciones para cada valor posible del dominio en esa posicion
    for(int i = 1; i<= tam; i++){
        if(dominio.enDominio(fil, col, i)){
           //Instanciamos el valor en esa posicion, (la funcion se describe mas adelante)
           xi= i;
           instanciar(podado, xi, fil, col, tam);
           
           //Caso base, cuando lleguemos a la ultima posicion (la 3,3), salimos
           if(fil == tam-1 and col==tam-1){
               return true;
           }
           else{//Si no, realizamos las comprobaciones

                //Si el valor cumple con la comprobacion de forward(), llamamos a la recursividad
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

                //Si el valor no cumple con forward o las llamadas a la recursividad devuelven               
                // falso, debemos restaurar el dominio
                restaura(podado, fil, col, tam);
            }
            if(i==tam){//Si ningun valor sirve, devolvemos falso
               return false;
            }


        }
        else{//Si el valor no esta en el dominio seguimos con el siguente
            continue;
        }
    }
    return false;
}


//FUNCION QUE REALIZA LA COMPROBACION HACIA ADELANTE
bool Solver::forward(Tablero *tablero, int fil, int col, int tam, int valor, matdominios &podado){
    
    // Comprobamos los valores en el dominio de las filas de adelante
    for(int fila_forward = fil+1; fila_forward < tam; fila_forward++){
        bool vacio = true; 
        for(int k=1; k <= tam; k++){
            //Primero comprobamos hacia adelante con las filas
            if(dominio.enDominio(fila_forward, col, k)){        
                //Vemos si el valor es consistente con el dominio de la casilla de adelante                
                if(consistente_fc(tablero, valor, fil, col, k, fila_forward, col)){
                    //Si es consistente, entonces el dominio no estara vacio
                    vacio = false;
                }
                else{
                    // el valor k no es consistente con la asignacion puesta, sacamos del dominio
                    // el valor para la casilla de forward y lo añadimos a la matriz de podados 
                    if(tablero->getCasilla(fila_forward, col) == 0){
                        dominio.sacarDominio(fila_forward, col, k);
                        podado.meterDominio(fila_forward,col, k);
                    }
                }
            }            
        }
        if(vacio){//Si ningun valir fue consistente, devolvemos un resultado falso
            return false;
        }
    }
    //Realizamos el mismo proceso de comprobacion pero ahora para las columnas de adelante
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

//FUNCION DE COMPROBACION DE CONSISTENCIA DE FORWARD CHECKING
//Su funcionamiento es el mismo que el de la de AC3, pero solo realiza la comprobacion para un valor de n2(el que comprobamos)
bool Solver::consistente_fc(Tablero *tablero, int num_Vk, int x_n1, int y_n1, int contando, int x_n2, int y_n2){
    bool resultado=false;
    float dist ;
    int dist2, arriba=0, abajo=0, derecha=0,izquierda=0, tamta = tablero->getSize();
    dist =sqrt(((x_n2-x_n1)*(x_n2-x_n1)) + ((y_n2-y_n1)*(y_n2-y_n1)));            
    if(num_Vk==contando){                
        return false;                
    }
    if(dist<=1.0){                                                               
        if(x_n1 == x_n2){
            dist2 = y_n2 - y_n1;
            if(dist2 == 1){
                if(y_n1 < tamta-1){
                    derecha = tablero->getElement(x_n1*2, 2*y_n1+1);
                }
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
                if(y_n1 > 0){
                    izquierda = tablero->getElement(2*x_n1, 2*y_n1-1);
                }   
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
                if(x_n1 > 0){
                    arriba = tablero->getElement(2*x_n1-1, y_n1*2); 
                }
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
                if(x_n1 < tamta-1){
                    abajo = tablero->getElement(2*x_n1+1, y_n1*2);
                }
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

//FUNCION PARA INSTANCIAR UN VALOR A UNA POSICIÓN DEL DOMINIO
void Solver::instanciar(matdominios &podado, int xi, int fil, int col, int tam){

    //Lo que hacemos es sacar del dominio de la posicion todos los valores menos el fijado
    // y de lamisma fila y columna sacamos el valor

    //Todo lo que saquemos del dominio, lo añadimos a la matriz de podados
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
}
//FUNCION PARA RESTAURAR EL DOMINIO CON LOS VALORES PODADOS POR LA INSTANCIANCIÓN Y POR FORWARD
void Solver::restaura(matdominios &podado, int fil, int col, int tam){

    //Recorremos toda la matriz en busca de los valores que fueron podados y los metemos de vuelta en el 
    // dominio 
    for(int i = fil; i<tam; i++){
        for(int j = col; j < tam; j++){
            for(int k = 1 ; k <= tam; k++){
                if(podado.enDominio(i,j, k)){
                    podado.sacarDominio(i, j, k);//Los sacamos de los podados
                    dominio.meterDominio(i, j, k);
                }
            }
        }
    }
}
