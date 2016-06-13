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

QString to_Send = "";
QString to_Display = "";
int nextLineCount = 0;
QString lineTrack = "";
bool finished_Clicking = false;

QPushButton **point; //pointer to dmfarray
int x;               //x,y coordinates
int y;
int newrow;
int newcolumn;
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

void DMFgui::autoGeneratePath(int rowI,int colI,int rowF, int colF, int path){

  //Determine number of electrodes to be turned on
  int size = abs(rowI-rowF)+abs(colI-colF);
  point[rowI][colI].setStyleSheet("background-color:yellow");



  //Create an array containing iCoordinates and jCoordinates
  int iCoord [size], jCoord [size];

  //Determines number of rows and columns needed to travel
  int rows = abs(rowI-rowF);
  int cols = abs(colI-colF);

  //Determines which direction the electrode is traveling (northwest,northeast,southwest,southeast
  bool rowCond = (rowF-rowI)>=0;
  bool colCond = (colF-colI)>=0;

  int a;

  //Northeast
  if (rowCond ==1 && colCond ==1){
      if (path == 1){
          for(a = 0; a<rows; a++){
              iCoord[a] = ++rowI;
              jCoord[a] = colI;
          }
          for(a;a<size;a++){
              iCoord[a] = rowF;
              jCoord[a] = ++colI;
          }
      }
      else if (path == 2){
          for(a = 0; a<cols; a++){
              iCoord[a] = rowI;
              jCoord[a] = ++colI;
          }
          for(a;a<size;a++){
              iCoord[a] = ++rowI;
              jCoord[a] = colF;
          }
      }
  }

  //
  else if (rowCond ==1 && colCond ==0){
      if (path == 1){
          for(a = 0; a<rows; a++){
              iCoord[a] = ++rowI;
              jCoord[a] = colI;
          }
          for(a;a<size;a++){
              iCoord[a] = rowF;
              jCoord[a] = --colI;
          }
      }
      else if (path == 2){
          for(a = 0; a<cols; a++){
              iCoord[a] = rowI;
              jCoord[a] = --colI;
          }
          for(a;a<size;a++){
              iCoord[a] = ++rowI;
              jCoord[a] = colF;
          }
      }
  }

  else if (rowCond ==0 && colCond ==0){
      if (path == 1){
          for(a = 0; a<rows; a++){
              iCoord[a] = --rowI;
              jCoord[a] = colI;
          }
          for(a;a<size;a++){
              iCoord[a] = rowF;
              jCoord[a] = --colI;
          }
      }
      else if (path == 2){
          for(a = 0; a<cols; a++){
              iCoord[a] = rowI;
              jCoord[a] = --colI;
          }
          for(a;a<size;a++){
              iCoord[a] = --rowI;
              jCoord[a] = colF;
          }
      }
  }

  else if (rowCond ==0 && colCond ==1){
      if (path == 1){
          for(a = 0; a<rows; a++){
              iCoord[a] = --rowI;
              jCoord[a] = colI;
          }
          for(a;a<size;a++){
              iCoord[a] = rowF;
              jCoord[a] = ++colI;
          }
      }
      else if (path == 2){
          for(a = 0; a<cols; a++){
              iCoord[a] = rowI;
              jCoord[a] = ++colI;
          }
          for(a;a<size;a++){
              iCoord[a] = --rowI;
              jCoord[a] = colF;
          }
      }
  }

  //cout << "\n";

  //Create array of electrodes to be turned on and store
  QString retMap [size];
  for (int m =0; m<size;m++){
      retMap[m] = point[iCoord[m]][jCoord[m]].text();
      point[iCoord[m]][jCoord[m]].setStyleSheet("background-color:green");      //Set electodes to be activated: green
      save_to_String(retMap[m]);
  }
  point[iCoord[size-1]][jCoord[size-1]].setStyleSheet("background-color:blue"); //Set last electode to be activated: blue



  //ui->textEdit->insertPlainText(retMap[0]+ " " +retMap[1]+ " " +retMap[2]+ " "+retMap[3]);

}
void DMFgui::ClearColor(){
//    if(autoGen == true){
//        for (int q=0;q<2;q++){
//            for(int r=0;r<2;r++){
//            //point[rcoord[0]][2].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
//            }
//        }
//    }
}



void DMFgui::on_exitButton_clicked()  //Temporarily used as autoGen button
{
    // Four inputs
    int a1=electrode_1.x;
    int a2=electrode_1.y;
    int a3=electrode_2.x;
    int a4=electrode_2.y;
    autoGeneratePath(a1,a2,a3,a4,1);
    //testing purposes: autoGen = true;

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
    QGridLayout *gridLayout = new QGridLayout;
    QSignalMapper *mapper = new QSignalMapper;
    QLabel display;

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
        newrow = drow+4;
        newcolumn = dcolumn+4;

        QPushButton **dmf_array = new QPushButton*[newrow];
                for(int i=0;i<newrow;i++){
                    dmf_array[i]=new QPushButton[newcolumn];
                }
        QLabel *empty = new QLabel(this);
        point = dmf_array;//dmf_array[3];

        //used for numbering the electrodes
        int numberingcount = 0;

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

                    //dmf_array.clicked.connect(save_to_String(dmf_array[i][j].text()));

                    QObject::connect(dmf_array[i],SIGNAL(clicked()),this, SLOT(save_to_String(dmf_array[i][j].text())));

                    mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                    mapper->setMapping(&dmf_array[i][j],QString::number(numberingcount));
                    mapper->setMapping(&dmf_array[i][j],QString::number(i)+QString::number(j)+QString::number(numberingcount));
                }
            }
        }

        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
        gridLayout->addWidget(&display,newrow,0,1,newcolumn);

        ui->graphicsView->setLayout(gridLayout);

        //display message to user to specify number of reservoirs
        bool ok;
        //QString text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);

//        if (text.toInt() == 0)
//        {
//            QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
//        }

//        if(ok&&!text.isEmpty())
//        {
//            //number of reservoirs
//            int resnum = text.toInt();
//            ui->textEdit->insertPlainText("\nplease select " + text + " reservoirs");

//            //get where the user clicked.
//            //int rowposition =**dmf_array.indexOf(sender());
//            //display window that gives them the option of where to add the reservoir (depending on where they clicked)
//            //send position of the array that they have selected
//            add_reservoir(resnum);

//        }

    }
}



void DMFgui::buttonClicked(QString text)
{
    //ClearColor(); Still testing

    //    ui->label->setText(x);
    //    ui->textEdit->setPlainText(ui->label->text());
    if (elec ==1)
    {
        electrode_1.x = text.at(0).unicode()-48; //converting from ASCII
        electrode_1.y = text.at(1).unicode()-48;

        //ui->textEdit->insertPlainText(QString::number(electrode_1.x));
        //ui->textEdit->insertPlainText(QString::number(electrode_1.y));
        ui->textEdit->insertPlainText("elec_1");

        elec ++; //go to electrode_2 next time a button is pressed
    }
    else if(elec==2)
    {
        electrode_2.x = text.at(0).unicode()-48;
        electrode_2.y = text.at(1).unicode()-48;

        //ui->textEdit->insertPlainText(QString::number(electrode_2.x));
       // ui->textEdit->insertPlainText(QString::number(electrode_2.y));
         ui->textEdit->insertPlainText("elec_2   path:  ");

        elec --; //go to electrode_1 next time a button is pressed
    }

//    save_to_String(text);
}


//displays and returns where to add the reservoirs
//allows user to select where they want the reservoir to be, or cancel
//keep track of how many times they've selected a reservoir
void DMFgui::add_reservoir(int resnum)
{
    //pop out a window

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


void DMFgui::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        ui->textEdit->insertPlainText("Pressed"); //double check this, it's appearing with the line edit
    }
}

void DMFgui::on_Voltage_SendButton_clicked()
{
    //read from the line edit then save as a float
   QString voltage = ui->lineEdit->text(); //gets the text that you've entered

    float to_Send = voltage.toFloat();
    //connect to the fgen class (generate the commands from there)
}




