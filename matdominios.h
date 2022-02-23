#ifndef _MATDOMINIO_
#define _MATDOMINIO_

class matdominios
{
private:
    /* data */
    int ancho;
    int alto;
    int profundo;
    int*** matriz;
public:

    matdominios(int);
    ~matdominios();
    bool enDominio(int, int, int);
    void sacarDominio(int, int, int);
    void meterDominio(int, int, int);
};




#endif