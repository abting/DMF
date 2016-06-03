#include "array_testing.h"
#include "ui_array_testing.h"
#include <QtWidgets>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>

Array_testing::Array_testing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Array_testing)
{
    ui->setupUi(this);

    int drow;
    int dcolumn;

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
    //creating new empty buttons
    QPushButton **dmf_array = new QPushButton*[drow];
            for(int i=0;i<drow;i++){
                dmf_array[i]=new QPushButton[dcolumn];
            }

    int count = 0;

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
        }
    }
    //once done, display the array of buttons on QtGraphicsView Widget
    display(drow,dcolumn, dmf_array);
}

//displays whatever was inputed to the QtGraphicsView Widget
void Array_testing::display(int row, int column,QPushButton **dmf_array)
{
//    QWidget *centralWidget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout;
    QGraphicsView *view = new QGraphicsView;
    QGraphicsScene *scene = new QGraphicsScene;

    view->setLayout(gridLayout);
    gridLayout->setParent(scene); //setting the parent of gridLayout to be the graphicsScene

    int total_buttons = row*column;

   QPushButton *button[5][5];
   button[1][0] = new QPushButton ("whatever");
   gridLayout->addWidget(button[1][0],1,0);


    //gridLayout->addWidget(dmf_array[1][1],1,1,1,1);
    for (int i=0;i<row; i++){
        for (int j=0;j<column;j++){
            gridLayout->addWidget(dmf_array[i][j],0,1);
        }
    }


   QPushButton *Button1 = new QPushButton;
   Button1->setText ("Button 1");

    //this works
    gridLayout->addWidget(Button1,0,1);

//    QGraphicsView view (&scene);
//    view.show();

}
