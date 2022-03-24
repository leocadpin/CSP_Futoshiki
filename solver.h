#ifndef SOLVER_H
#define SOLVER_H

#include "tablero.h"
#include "matdominios.h"
#include <QObject>

class Solver : public QObject
{
    Q_OBJECT
public:
    explicit Solver(QObject *parent = nullptr);

    void ejecutarBT(Tablero *);
    void ejecutarAC3(Tablero *);
    void ejecutarFC(Tablero *);

    void instanciar(matdominios &, int, int, int, int);
    void restaura( matdominios &, int, int, int);
    bool consistente_fc(Tablero *, int , int , int , int , int , int);
    bool bt_futoshiki(Tablero *, int, int, int);
    bool fc_futoshiki(Tablero *, int, int, int);
    bool factible(Tablero *, int , int, int , int);
    bool forward(Tablero *, int, int , int, int, matdominios &);
    bool consistente(Tablero *, int, int, int, int, int);
signals:

public slots:

private:
    int backTrackingJumps;
};

#endif // SOLVER_H
