#ifndef _MATDOMINIO_
#define _MATDOMINIO_


//CLASE MATDOMINIOS, REPRESENTA UNA MATRIZ DE TRES DIMENSIONES PARA EL DOMINIO DEL PROBLEMA
//LAS DIMENSIONES 1 Y 2 REPRESENTAN LA POSICION DE FILA Y COLUMNA DEL TABLERO,
//LA DIMENSION 3 REPRESENTA EL NUMERO Y EL VALOR ES 1 O 0 DEPENDIENDO DE SI ESTA EN EL DOMINIO
class matdominios
{
private:
    /* data */
    int ancho;
    int alto;
    int profundo;
    int*** matriz;
public:
    void resize_matdominios(int);
    matdominios(int);
    matdominios(int, int);
    ~matdominios();
    bool enDominio(int, int, int);
    void sacarDominio(int, int, int);
    void meterDominio(int, int, int);
    void imprimirDominio();
};




#endif