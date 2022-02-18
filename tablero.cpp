#include "tablero.h"

#include <QFile>
#include <QTextStream>

using namespace std;

/**
 * @brief Tablero::Tablero Constructor por defecto de la clase tablero
 * @param parent
 * Crea un objeto tablero vacío con tamaño máximo 17x17. En las posiciones
 * pares, tanto de fila como de columna, se almacenarán los valores de las
 * celdas y en las impares la relación binaria entre ellas.
 *
 * La codificación de las relaciones binarias es:
 *   1: A < B
 *   0: sin relación
 *  -1: A > B
 */
Tablero::Tablero(QObject *parent) : QObject(parent)
{
    int fila, col;
    int maxSize = TAM_MAX*2-1;

    size = 0;

    m_tablero = new int*[maxSize];

    for(fila=0;fila<maxSize;fila++)
    {
        m_tablero[fila] = new int[maxSize];
        for(col=0;col<maxSize;col++)
            m_tablero[fila][col] = 0;
    }
}

Tablero::~Tablero()
{
    int cont;
    int maxSize = TAM_MAX*2-1;
    for(cont=0;cont<maxSize;cont++)
        delete m_tablero[cont];
    delete m_tablero;
}

/**
 * @brief Tablero::getCasilla Obtiene el valor de una casilla del tablero
 * @param fila Fila de la casilla que queremos obtener [0..size-1]
 * @param col Columna de la casilla que queremos obtener [0..size-1]
 * @return El valor de la casilla del tablero especificada por @param fila y @param col
 */
int Tablero::getCasilla(int fila, int col)
{
    return m_tablero[fila*2][col*2];
}

/**
 * @brief Tablero::setCasilla Establece un valor en una casilla del tablero
 * @param fila Fila de la casilla que queremos modificar [0..size-1]
 * @param col Columna de la casilla que queremos modificar [0..size-1]
 * @param valor Nuevo valor para la casilla especificada por @param fila y @param col
 */
void Tablero::setCasilla(int fila, int col, int valor)
{
    m_tablero[fila*2][col*2] = valor;
    return;
}

/**
 * @brief Tablero::getElement Obtiene un elemento de la representación interna del tablero. Si el tamaño del tablero
 * es (size x size) el tamaño de la representación interna es 2*size-1.
 * @param fila Fila del tablero que queremos leer [0..2*size-1]
 * @param col Columna del tablero que queremos leer [0..2*size-1]
 * @return
 */
int Tablero::getElement(int fila, int col)
{
    return m_tablero[fila][col];
}

/**
 * @brief tablero::setFichero Carga los valores del tablero desde un fichero de texto.
 * @param fileName El nombre del fichero desde donde se cargarán los valores del tablero
 */
void Tablero::setFichero(QString fileName)
{
   int fila, col, maxSize;
   QFile inputFile(fileName);
   inputFile.open(QIODevice::ReadOnly);

   QTextStream in(&inputFile);
   in >> size;

   maxSize = size*2-1;
   for(fila=0;fila<maxSize;fila++)
       for(col=0;col<maxSize;col++)
           in >> m_tablero[fila][col];

   inputFile.close();
   return;

}

/**
 * @brief Tablero::tableroCompleto Comprueba si se ha completado el tablero
 * @return true si el tablero está completo o false en otro caso
 */
bool Tablero::tableroCompleto()
{
    int fila, col, maxSize;
    bool completo = true;
    maxSize = size*2-1;

    for(fila=0;fila<maxSize && completo; fila+=2)
        for(col=0;col<maxSize && completo; col+=2)
            if(m_tablero[fila][col]==0) completo = false;

    return completo;
}

/**
 * @brief Tablero::tableroCorrecto Comprueba que se cumplan todas las restricciones del tablero
 * @return true si el tablero es correcto
 */
bool Tablero::tableroCorrecto()
{
    bool ret = !tableroCompleto();

    int fila, col;

    for(fila=0; fila<size && !ret; fila++)
        for(col=0; col<size && !ret; col++)
            ret = estaEnFila(fila, col) || estaEnCol(fila, col) || comprobarBinariaSiguienteHorizontal(fila, col)
                    || comprobarBinariaSiguienteVertical(fila, col);

    return !ret;
}


/**
 * @brief Tablero::estaEnFila comprueba si el valor del tablero en la posicion (@param fila, @param col) se repite en la fila @param fila
 * @param fila Fila de la casilla en el tablero [0..size-1]
 * @param col Columna de la casilla en el tablero [0..size-1]
 * @return true si el valor de (@param fila, @param col) se repite en la fila @param fila, false en otro caso
 */
bool Tablero::estaEnFila(int fila, int col)
{
    bool ret = false;
    int iter, valor, maxSize;
    int filaActual = fila*2;
    int colActual = col*2;

    valor = m_tablero[filaActual][colActual];
    maxSize = size*2-1;

    for(iter=0; iter<maxSize && !ret; iter+=2)
        if(iter!=colActual && m_tablero[filaActual][iter]==valor) ret = true;

    return ret;
}

/**
 * @brief Tablero::estaEnCol comprueba si el valor del tablero en la posicion (@param fila, @param col) se repite en la columna @param col
 * @param fila Fila de la casilla en el tablero [0..size-1]
 * @param col Columna de la casilla en el tablero [0..size-1]
 * @return true si el valor de (@param fila, @param col) se repite en la columna @param col, false en otro caso
 */
bool Tablero::estaEnCol(int fila, int col)
{
    bool ret = false;
    int iter, valor, maxSize;
    int filaActual = fila*2;
    int colActual = col*2;

    valor = m_tablero[filaActual][colActual];
    maxSize = size*2-1;

    for(iter=0; iter<maxSize && !ret; iter+=2)
        if(iter!=filaActual && m_tablero[iter][colActual]==valor) ret = true;

    return ret;

}

/**
 * @brief Tablero::comprobarBinariaSiguienteVertical A partir de una posición en el tablero comprueba la restricción binaria con la siguiente casilla en vertical
 * @param fila Fila de la casilla en el tablero [0..size-1]
 * @param col Columna de la casilla en el tablero [0..size-1]
 * @return true en caso de que NO se cumpla la relación binaria. False en caso de que se cumpla, no exista la restricción binaria o una de las casillas esté vacía
 */
bool Tablero::comprobarBinariaSiguienteVertical(int fila, int col)
{
    bool ret = false;
    int colActual = col*2;
    int filaActual = fila*2;
    int filaSig = filaActual+2;
    int binaria;

    //controlamos no salirnos del tablero, que realmente existe una restricción binaria y que las casillas afectadas no estén vacías
    if(filaSig<size*2-1 && m_tablero[filaActual+1][colActual]!=0 && m_tablero[filaActual][colActual]!=0 && m_tablero[filaSig][colActual]!=0)
    {
        binaria = m_tablero[filaActual+1][colActual];

        if ((binaria==1&&m_tablero[filaActual][colActual]>m_tablero[filaSig][colActual])
                || (binaria==-1&&m_tablero[filaActual][colActual]<m_tablero[filaSig][colActual]))
            ret = true;
    }

    return ret;
}

/**
 * @brief Tablero::comprobarBinariaSiguienteHorizontal A partir de una posición en el tablero comprueba la restricción binaria con la siguiente casilla en horizontal
 * @param fila Fila de la casilla en el tablero [0..size-1]
 * @param col Columna de la casilla en el tablero [0..size-1]
 * @return true en caso de que NO se cumpla la relación binaria. False en caso de que se cumpla, no exista la restricción binaria o una de las casillas esté vacía
 */
bool Tablero::comprobarBinariaSiguienteHorizontal(int fila, int col)
{
    bool ret = false;
    int colActual = col*2;
    int filaActual = fila*2;
    int colSig = colActual+2;
    int binaria;

    //controlamos no salirnos del tablero, que realmente existe una restricción binaria y que las casillas afectadas no estén vacías
    if(colSig<size*2-1 && m_tablero[filaActual][colActual+1]!=0 && m_tablero[filaActual][colActual]!=0 && m_tablero[filaActual][colSig]!=0)
    {
        binaria = m_tablero[filaActual][colActual+1];

        if ((binaria==1&&m_tablero[filaActual][colActual]>m_tablero[filaActual][colSig])
                || (binaria==-1&&m_tablero[filaActual][colActual]<m_tablero[filaActual][colSig]))
            ret = true;
    }

    return ret;

}

/**
 * @brief Tablero::getSize Obtiene el tamaño del tablero. El número de casillas en cada fila/columna.
 * @return Tamaño del tablero
 */
int Tablero::getSize() const
{
    return size;
}
