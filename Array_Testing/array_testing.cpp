#include "array_testing.h"
#include "ui_array_testing.h"
#include <QtWidgets>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>

Array_testing::Array_testing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Array_testing)
{
    ui->setupUi(this);

    //gets saved to for an arrayList
    //name components in the arrayList and dynamically create a button for every slot

    //displaying the array on the screen

    //somehow have the user determine where each entry place is, give options of reservoirs

    //autogenerate paths
}

Array_testing::~Array_testing()
{
    delete ui;
}

//sends the information on the numbers of rows and columns then prints it on the screen
void Array_testing::on_enterButton_clicked()
{
    //get the texts from the textEdits
    QString row = ui->rowEdit->text();
    QString column = ui->columnEdit->text();

    int drow = row.toDouble();
    int dcolumn = column.toDouble();

    //Dynamically create an array
    QWidget *centralWidget = new QWidget;
    QPushButton *dmf_array[drow][dcolumn];
    QGridLayout *gridLayout = new QGridLayout;
//    std::vector<std::vector<QPushButton>> dmf_array;

//    dmf_array.resize(drow);
//    for (int i=0;i<drow;i++)
//    {
//        dmf_array[i].resize(dcolumn);
//    }

    int count = -1;
    std::ostringstream button("");

    //assign a button to each spot, going from left to right, top to bottom
    for (int i=0;i<drow;i++)
    {
        count++;
        for (int j=0;j<dcolumn;j++)
        {
            //formulating the name
            int numbering = count*dcolumn;

            button<<numbering;

            QPushButton *button.str().c_str() = new QPushButton(numbering+j);
//            dmf_array[i][j] == button_%numbering;
        }
    }
}

//displays whatever was inputed to the QtGraphicsView Widget
void Array_testing::display()
{

}
