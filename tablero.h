#ifndef TABLERO_H
#define TABLERO_H

#include<iostream>
#include <QObject>

#define TAM_MAX 9

class Tablero : public QObject
{
    Q_OBJECT
public:
    explicit Tablero(QObject *parent = nullptr);
    ~Tablero();

    int getCasilla(int fila, int col);
    void setCasilla(int fila, int col, int valor);
    int getElement(int fila, int col);
    void setFichero(QString fileName);
    bool tableroCompleto();
    bool tableroCorrecto();
    bool estaEnFila(int fila, int col);
    bool estaEnCol(int fila, int col);
    bool comprobarBinariaSiguienteHorizontal(int fila, int col);
    bool comprobarBinariaSiguienteVertical(int fila, int col);

    int getSize() const;

private:
    int **m_tablero;
    int size;


signals:

public slots:
};

#endif // TABLERO_H
