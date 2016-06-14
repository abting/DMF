#include "dmfgui.h"
#include "ui_dmfgui.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <QSignalMapper>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <climits>

QPushButton **dmf_array;
QGridLayout *gridLayout;
QSignalMapper *mapper;

//used for numbering the electrodes
int numberingcount = 0;
int newrow,newcolumn;
int resnum;
int added = 0;
int corner;

QString to_Send = "";
QString to_Display = "";
int nextLineCount = 0;
QString lineTrack = "";
bool finished_Clicking = false;
bool enter_Button_Clicked =false;

QPushButton **point;

bool addRes = false;
int x;
int y;

int* rcoord;
int* ccoord;
int track[4];
int size1;

struct coordinates
{
    int x;
    int y;
}electrode_1,electrode_2;

int elec = 1;

DMFgui::DMFgui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DMFgui)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";

    ui->textEdit->setReadOnly(true); //not allowing user to change anything

    //defining arduino here
    arduino=new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier()&& serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)
            {
                if (serialPortInfo.productIdentifier()==arduino_uno_product_id)
                {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }
    if (arduino_is_available){
        //open and configure the serialport
        //set the port name
        arduino->setPortName(arduino_port_name);
        //open it, only sending command, not writing anything (so writeOnly)
        arduino->open(QSerialPort::WriteOnly);
        //make sure that Baud rate matches what's on the arduino
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

    }
    else{
        //give an error message, (no arduino available)
        QMessageBox::warning(this,"Port error", "Couldn't find the Arduino!");
    }
}

DMFgui::~DMFgui()
{
    if (arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
}

void DMFgui::save_to_String(QString electrode_num)
{
    to_Send += electrode_num + ",";

    if (electrode_num=="1000" || electrode_num=="1001"|| electrode_num=="1003")
    {
        ui->textEdit->insertPlainText("\n"+electrode_num + ",");
    }
    else
    {
        ui->textEdit->insertPlainText(""+electrode_num + ","); //find a way to have this deleted one by one
    }
}
void DMFgui::on_mixButton_clicked()
{
    save_to_String("1003");
}
void DMFgui::on_sequenceButton_clicked()
{
    save_to_String("1000");
}
void DMFgui::on_splitButton_clicked()
{
    save_to_String("1001");
}
void DMFgui::on_resetButton_clicked()
{
    to_Send = "";
    ui->textEdit->clear();
}
//void DMFgui::autoGeneratePath(int rowI,int colI,int rowF, int colF, int path){

//  //Determine number of electrodes to be turned on
//  int size = abs(rowI-rowF)+abs(colI-colF);
//  point[rowI][colI].setStyleSheet("background-color:yellow");



//  //Create an array containing iCoordinates and jCoordinates
//  int iCoord [size], jCoord [size];

//  //Determines number of rows and columns needed to travel
//  int rows = abs(rowI-rowF);
//  int cols = abs(colI-colF);

//  //Determines which direction the electrode is traveling (northwest,northeast,southwest,southeast
//  bool rowCond = (rowF-rowI)>=0;
//  bool colCond = (colF-colI)>=0;

//  int a;

//  //Northeast
//  if (rowCond ==1 && colCond ==1){
//      if (path == 1){
//          for(a = 0; a<rows; a++){
//              iCoord[a] = ++rowI;
//              jCoord[a] = colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = rowF;
//              jCoord[a] = ++colI;
//          }
//      }
//      else if (path == 2){
//          for(a = 0; a<cols; a++){
//              iCoord[a] = rowI;
//              jCoord[a] = ++colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = ++rowI;
//              jCoord[a] = colF;
//          }
//      }
//  }

//  //
//  else if (rowCond ==1 && colCond ==0){
//      if (path == 1){
//          for(a = 0; a<rows; a++){
//              iCoord[a] = ++rowI;
//              jCoord[a] = colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = rowF;
//              jCoord[a] = --colI;
//          }
//      }
//      else if (path == 2){
//          for(a = 0; a<cols; a++){
//              iCoord[a] = rowI;
//              jCoord[a] = --colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = ++rowI;
//              jCoord[a] = colF;
//          }
//      }
//  }

//  else if (rowCond ==0 && colCond ==0){
//      if (path == 1){
//          for(a = 0; a<rows; a++){
//              iCoord[a] = --rowI;
//              jCoord[a] = colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = rowF;
//              jCoord[a] = --colI;
//          }
//      }
//      else if (path == 2){
//          for(a = 0; a<cols; a++){
//              iCoord[a] = rowI;
//              jCoord[a] = --colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = --rowI;
//              jCoord[a] = colF;
//          }
//      }
//  }

//  else if (rowCond ==0 && colCond ==1){
//      if (path == 1){
//          for(a = 0; a<rows; a++){
//              iCoord[a] = --rowI;
//              jCoord[a] = colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = rowF;
//              jCoord[a] = ++colI;
//          }
//      }
//      else if (path == 2){
//          for(a = 0; a<cols; a++){
//              iCoord[a] = rowI;
//              jCoord[a] = ++colI;
//          }
//          for(a;a<size;a++){
//              iCoord[a] = --rowI;
//              jCoord[a] = colF;
//          }
//      }
//  }

//  //cout << "\n";

//  //Create array of electrodes to be turned on and store
//  QString retMap [size];
//  for (int m =0; m<size;m++){
//      retMap[m] = point[iCoord[m]][jCoord[m]].text();
//      point[iCoord[m]][jCoord[m]].setStyleSheet("background-color:green");      //Set electodes to be activated: green
//      save_to_String(retMap[m]);
//  }
//  point[iCoord[size-1]][jCoord[size-1]].setStyleSheet("background-color:blue"); //Set last electode to be activated: blue



//  //ui->textEdit->insertPlainText(retMap[0]+ " " +retMap[1]+ " " +retMap[2]+ " "+retMap[3]);

//}
void DMFgui::ClearColor(){
//    if(autoGen == true){
//        for (int q=0;q<2;q++){
//            for(int r=0;r<2;r++){
//            //point[rcoord[0]][2].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
//            }
//        }
//    }
}
void DMFgui::on_exitButton_clicked()
{

}

void DMFgui::on_sendButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Are you sure","Are you sure?",QMessageBox::Yes|QMessageBox::No);

    //send info if user is true
    if (reply == QMessageBox::Yes)
    {
        ui->textEdit->insertPlainText("\nSending...");

        updateDMF(to_Send);

        ui->textEdit->insertPlainText("\nSent!");
        ui->textEdit->insertPlainText(to_Send);
    }
    //automatically go back if user is not sure
}


void DMFgui::on_enterButton_clicked()
{

    //get the texts from the textEdits
    QString row = ui->rowEdit->text();
    QString column = ui->columnEdit->text();
    gridLayout = new QGridLayout;
    mapper = new QSignalMapper;
    QLabel display;

    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setSpacing(0);

    int drow = row.toDouble();
    int dcolumn = column.toDouble();

    if(!enter_Button_Clicked)
    {
        //value entered is not in int, pop up an error message
        if (row.toInt() == 0||column.toInt()==0)
        {
            QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
        }

        else
        {
            //Dynamically create an array
            //creating new empty buttons (array of array)
            newrow = drow+4;
            newcolumn = dcolumn+4;

            dmf_array = new QPushButton*[newrow];
                    for(int i=0;i<newrow;i++){
                        dmf_array[i]=new QPushButton[newcolumn];
                    }

            QLabel *empty = new QLabel(this);

            //naming each of the electrodes
            for (int i=0;i<newrow;i++)
            {
                //count++;
                for (int j=0;j<newcolumn;j++)
                {
                    //set the edge spaces as null
                    if (i==0||i==1||j==0||j==1||i==drow+3||i==drow+2||j==dcolumn+3||j==dcolumn+2)
                    {
                        gridLayout->addWidget(empty,i,j);
                    }

                    else
                    {
                        numberingcount++;

                        //inserting values into the array
                        dmf_array[i][j].setText(QString::number(numberingcount));
                        dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                        gridLayout->addWidget(&dmf_array[i][j],i,j);

                        mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                        mapper->setMapping(&dmf_array[i][j],QString::number(i)+QString::number(j)+QString::number(numberingcount));
                    }
                }
            }

            connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
            gridLayout->addWidget(&display,newrow,0,1,newcolumn);

            ui->graphicsView->setLayout(gridLayout);

            //display message to user to specify number of reservoirs
            bool ok;
            QString text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);

            if (text.toInt() == 0)
            {
                QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
            }

            if(ok&&!text.isEmpty())
            {
                //number of reservoirs
                resnum = text.toInt();
                ui->textEdit->insertPlainText("\nplease select " + text + " reservoirs");

                //display window that gives them the option of where to add the reservoir (depending on where they clicked)
                //send position of the array that they have selected

                addRes = true;
            }

        }
        enter_Button_Clicked = true;
    }
    else
    {
        QMessageBox::warning(this,tr("Invalid"), tr("You can't click on this button again"));
    }
}

void DMFgui::buttonClicked(QString text)
{
//    ui->label->setText(x);
//    ui->textEdit->setPlainText(ui->label->text());
    if (elec ==1)
    {
        electrode_1.x = text.at(1).unicode()-48; //converting from ASCII
        electrode_1.y = text.at(0).unicode()-48;

        ui->textEdit->insertPlainText("\n electrode: " +QString::number(text.at(2).unicode()-48));
        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_1.x));
        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_1.y));

        elec ++; //go to electrode_2 next time a button is pressed
    }
    else if(elec==2)
    {
        electrode_2.x = text.at(1).unicode()-48;
        electrode_2.y = text.at(0).unicode()-48;

        ui->textEdit->insertPlainText("\n electrode: " +QString::number(text.at(2).unicode()-48));
        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_2.x));
        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_2.y));

        elec --; //go to electrode_1 next time a button is pressed
    }

    //if addReservoir was called
    if (addRes)
    {
        ui->textEdit->insertPlainText("\noutside the big loop: " + QString::number(added));
        if (added<resnum)
        {
           if (add_reservoir(newcolumn,newrow,resnum))
           {
               added++;
               ui->textEdit->insertPlainText("\noutside the small loop: " + QString::number(added));
           }
        }
        else
        {
            addRes = false;
        }
    }

//    save_to_String(text);
}

bool DMFgui::add_reservoir(int column, int row, int resnum)
{
    //getting the most recent coordinates of pressed electrodes
    int x_coord = getRecent_x_Coordinate();
    int y_coord = getRecent_y_Coordinate();

    QPushButton *reservoir = new QPushButton;
    reservoir->setText("res");
    reservoir->setEnabled(false);//cannot press this button

    QPushButton *extra_elec = new QPushButton;
    extra_elec->setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");

    bool case2_out;

    int caseSwitch;

    //the cases work fine. They're accurate
    if ((x_coord==2&&y_coord==2)||(x_coord==2&&y_coord==(row-3))||(x_coord==(column-3)&&y_coord==(row-3))||(x_coord==(column-3)&&y_coord==2))
    {
        caseSwitch = 2; //corner
        ui->textEdit->insertPlainText("\n case 2");
    }
    else if (x_coord==2||x_coord==(column-3)||y_coord==2||y_coord==(row-3))
    {
        caseSwitch = 1; //not a corner
        ui->textEdit->insertPlainText("\n case 1");
    }
    else
    {
        caseSwitch = 0; //not a valid reservoir
        ui->textEdit->insertPlainText("\n case 0");
    }

    switch (caseSwitch)
    {
        case 2:
            numberingcount++;
            extra_elec->setText(QString::number(numberingcount));

            //ask user if they want a reservoir on the left or on the right
            //make a separate case for each 4 corners
            if (x_coord==2&&y_coord==2)
            {
                ui->textEdit->insertPlainText("\n going in top-left");
                corner = 1; //"top-left"
            }
            else if (x_coord==2&&y_coord==(row-3))
            {
                ui->textEdit->insertPlainText("\n going in bottom-left");
                corner = 4;//"bottom-left"
            }
            else if (x_coord==(column-3)&&y_coord==(row-3))
            {
                ui->textEdit->insertPlainText("\n going in bottom-right");
                corner = 3;//"bottom-right"
            }
            else if (x_coord==(column-3)&&y_coord==2)
            {
                ui->textEdit->insertPlainText("\n got in of top-right");
                corner = 2;//"top-right"
            }

            if (openNewWindow(corner)=="top")
            {
                gridLayout->addWidget(extra_elec,1,x_coord);
                gridLayout->addWidget(reservoir,0,x_coord);
                return true;
                break;
            }
            else if (openNewWindow(corner)=="left")
            {
                gridLayout->addWidget(extra_elec,y_coord,1);
                gridLayout->addWidget(reservoir,y_coord,0);
                return true;
                break;
            }
            else if (openNewWindow(corner)=="right")
            {
                gridLayout->addWidget(extra_elec,y_coord,row-2);
                gridLayout->addWidget(reservoir,y_coord,row-1);
                return true;
                break;
            }
            else if (openNewWindow(corner)=="bottom")
            {
                gridLayout->addWidget(extra_elec,column-2,x_coord);
                gridLayout->addWidget(reservoir,column-1,x_coord);
                return true;
                break;
            }
            return true;
            break;
        case 1:
            //add new buttons depending on where the button clicked is (reservoir is hatched)
                numberingcount++;
                extra_elec->setText(QString::number(numberingcount));

                //set booleans so that you don't get to click to same place twice
                if (x_coord==2)//left column
                {
                    gridLayout->addWidget(extra_elec,y_coord,1);
                    gridLayout->addWidget(reservoir,y_coord,0);
                    return true;
                    break;
                }
                else if (x_coord==(column-3)) //right column
                {
                    gridLayout->addWidget(extra_elec,y_coord,column-2);
                    gridLayout->addWidget(reservoir,y_coord,column-1);
                    return true;
                    break;
                }
                else if (y_coord==2)
                {
                    gridLayout->addWidget(extra_elec,1,x_coord);
                    gridLayout->addWidget(reservoir,0,x_coord);
                    return true;
                    break;
                }
                else if(y_coord==(row-3))
                {
                    gridLayout->addWidget(extra_elec,row-2,x_coord);
                    gridLayout->addWidget(reservoir,row-1,x_coord);
                    return true;
                    break;
                }

                mapper->connect(extra_elec,SIGNAL(clicked()),mapper,SLOT(map()));
                mapper->setMapping(extra_elec,QString::number(x_coord-1)+QString::number(y_coord)+QString::number(numberingcount));

        case 0:
            QMessageBox::warning(this,tr("Invalid"), tr("You can't put a reservoir here"));
            return false;
            break;
    }
}

//getting the most recent x coordinate of electrode that was clicked
int DMFgui::getRecent_x_Coordinate()
{
    int x=0;
    if (elec==1)
    {
        x = electrode_2.x;
    }
    else if (elec==2)
    {
        x = electrode_1.x;
    }
    return x;
}

int DMFgui::getRecent_y_Coordinate()
{
    int y =0;
    if (elec==1)
    {
        y = electrode_2.y;
    }
    else if (elec==2)
    {
        y = electrode_1.y;
    }
    return y;
}

void DMFgui::updateDMF(QString to_Send)
{
    if (arduino->isWritable())
    {
        arduino->write(to_Send.toStdString().c_str());
    }
    else
    {
        qDebug() << "Couldn't write to serial";
    }
}

void DMFgui::on_UndoButton_clicked()
{
    ui->textEdit->undo(); //find a way to delete one by one.
}

//void DMFgui::set_Scene()
//{

//    //loading the picture from computer
//    QString imagePath = QFileDialog::getOpenFileName(this,
//                                                     tr("OpenFile"),
//                                                     "",
//                                                     tr("JPEG(*.jpg *.jpeg);;PNG(*.png)")
//                                                     );

//    imageObject = new QImage();
//    imageObject->load(":/DMF_Scene.png");

//    image = QPixmap::fromImage(*imageObject);

//    scene = new QGraphicsScene(this);
//    scene->addPixmap(image);
//    scene->setSceneRect(image.rect());
//    ui->graphicsView->setScene(scene);
//    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

//    //manually creating a button
//    QPushButton *buttonWidget = new QPushButton;
//    buttonWidget->setMinimumWidth(200);
//    buttonWidget->setMinimumHeight(150);
//    buttonWidget->setStyleSheet("background-color: white");
//    buttonWidget->setText(QString("Test"));
//    scene->addWidget(buttonWidget)->moveBy(40,50);
//    ui->graphicsView->setScene(scene);
//}

//void DMFgui::on_loadButton_clicked()
//{
//    set_Scene();
//}

QString DMFgui::openNewWindow(int corner)
{
    dialog = new Dialog();
    dialog->show();
    if (corner == 1)//"top-left"
    {
        ui->textEdit->insertPlainText("\n got out of top-left");
        dialog->topLeft();
        ui->textEdit->insertPlainText(dialog->on_buttonBox_accepted());
        return dialog->on_buttonBox_accepted();
    }
    else if (corner == 2)//"top-right"
    {
        ui->textEdit->insertPlainText("\n got out of top-right");
        dialog->topRight();
        ui->textEdit->insertPlainText(dialog->on_buttonBox_accepted());
        return dialog->on_buttonBox_accepted();
    }
    else if (corner == 3)//"bottom-right"
    {
        ui->textEdit->insertPlainText("\n got out of bottom-right");
        dialog->bottomRight();
        ui->textEdit->insertPlainText(dialog->on_buttonBox_accepted());
        return dialog->on_buttonBox_accepted();
    }
    else if(corner == 4)//"bottom-left"
    {
        ui->textEdit->insertPlainText("\n got out of bottom-left");
        dialog->bottomLeft();
        ui->textEdit->insertPlainText(dialog->on_buttonBox_accepted());
        return dialog->on_buttonBox_accepted();
    }

    return "nothing";
}

void DMFgui::mousePressEvent(QMouseEvent *e)
{
//    if (e->button() == Qt::LeftButton)
//    {
//        ui->textEdit->insertPlainText("Pressed"); //double check this, it's appearing with the line edit
//    }
}

void DMFgui::on_Voltage_SendButton_clicked()
{
    //read from the line edit then save as a float
   QString voltage = ui->lineEdit->text(); //gets the text that you've entered

    float to_Send = voltage.toFloat();
    //connect to the fgen class (generate the commands from there)
}

void DMFgui::on_autogen_Button_clicked()
{
    // Four inputs
        int a1=electrode_1.x;
        int a2=electrode_1.y;
        int a3=electrode_2.x;
        int a4=electrode_2.y;
//        autoGeneratePath(a1,a2,a3,a4,1);
        //testing purposes: autoGen = true;
}
