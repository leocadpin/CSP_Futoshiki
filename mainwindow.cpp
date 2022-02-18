#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    miTablero = new Tablero();
    solver = new Solver();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete miTablero;
    if (solver) delete solver;
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName;
    QString msg;
    QString name;

    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Futoshiki"), "../CSP_Futoshiki/tableros", tr("Futoshiki Files (*.txt)"));
    if(fileName!=nullptr)
        miTablero->setFichero(fileName);

    reDraw();
    msg.append("Estado: desconocido\nSolución no comprobada");
    ui->statusLabel->setText(msg);
    name = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    ui->filenameLabel->setText(name);
    if(solver) delete solver;
    solver = new Solver();
    return;

}

void MainWindow::on_solveButton_clicked()
{
    if(miTablero->getSize()==0)
        ui->statusLabel->setText("Error: Tablero\n no cargado.");
    else
    {
        if(ui->BTradioButton->isChecked())
            solver->ejecutarBT(miTablero);
        else if(ui->AC3radioButton->isChecked())
            solver->ejecutarAC3(miTablero);
        else
            solver->ejecutarFC(miTablero);
        reDraw();
        if(miTablero->tableroCorrecto())
        {
            ui->statusLabel->setText("Estado: resuelto\nSolución correcta");
        }
        else {
            ui->statusLabel->setText("Estado: resuelto\nSolución INCORRECTA");
        }
    }
    return;
}


void MainWindow::reDraw()
{
    int fila, col, valor;
    int maxSize = miTablero->getSize()*2-1;
    QString msg = "<table bgcolor='#7A7A7A'>";

    for(fila=0; fila<maxSize; fila++)
    {
        msg.append("<tr>");
        for(col=0; col<maxSize; col++)
        {
            if(fila%2==0 && col%2==0)
                msg.append("<td bgcolor='#FAFAFA' valign='center'>");
            else
                msg.append("<td valign='center'>");

            valor = miTablero->getElement(fila, col);

            if(valor==0)
            {
                if(fila%2==0 && col%2==0)
                    msg.append("<b><big>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</big></b>");
                else
                    msg.append("&nbsp;");
            }
            else {
                if(fila%2==0 && col%2==0)
                {
                    msg.append("<b><big>&nbsp;&nbsp;");
                    msg.append(QString::number(valor));
                    msg.append("</big></b>&nbsp;&nbsp;");
                }
                else
                    if(fila%2==0)
                        if(valor==1)
                            msg.append("<center>&lt;</center>");
                        else
                            msg.append("<center>&gt;</center>");
                    else if(valor==1)
                        msg.append("<center><b>^</b></center>");
                    else
                        msg.append("<center>v</center>");

            }
            msg.append("</td>");
        }
        msg.append("</tr>");
    }
    msg.append("</table>");

    ui->tableroLabel->setText(msg);

}
