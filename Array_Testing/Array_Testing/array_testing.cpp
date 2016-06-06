#include "array_testing.h"
#include "ui_array_testing.h"
#include <QtWidgets>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <QMessageBox>

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

//sends the information on the numbers of rows and columns then prints array on the screen
void Array_testing::on_enterButton_clicked()
{

    //get the texts from the textEdits
    QString row = ui->rowEdit->text();
    QString column = ui->columnEdit->text();
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setSpacing(0);

    int drow = row.toDouble();
    int dcolumn = column.toDouble();

    //value entered is not in int, pop up an error message
    if (row.toInt() == 0||column.toInt()==0)
    {
        QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
    }

    else
    {
        //Dynamically create an array
        //creating new empty buttons (array of array)
        QPushButton **dmf_array = new QPushButton*[drow];
                for(int i=0;i<drow;i++){
                    dmf_array[i]=new QPushButton[dcolumn];
                }

        int count = -1;

        //naming each of the electrodes
        for (int i=0;i<drow;i++)
        {
            count++;
            for (int j=0;j<dcolumn;j++)
            {
                //used for numbering the electrodes
                int numbering = count*dcolumn;

                //inserting values into the array
                dmf_array[i][j].setText(QString::number(numbering+j));
                gridLayout->addWidget(&dmf_array[i][j],i,j);
            }
        }
        ui->graphicsView->setLayout(gridLayout);
    }
}

//create function to autogenerate path
    //inside function, highlight the path by setting button colors using: ButtonName.setStyleSheet( "background-color: ...");
    //give user option to change the path
    //send final information
