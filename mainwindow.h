#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "solver.h"
#include "tablero.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();

    void on_solveButton_clicked();


private:
    Ui::MainWindow *ui;
    Tablero *miTablero;
    Solver *solver;

    void reDraw();
};

#endif // MAINWINDOW_H
