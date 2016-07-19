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
#include <QCloseEvent>
#include <QFile>
#include <time.h>
#include <algorithm>


QPushButton **dmf_array;
QGridLayout *gridLayout;
QSignalMapper *mapper;

//used for numbering the electrodes
int numberingcount = 0;
int newrow,newcolumn,resnum,corner;
int added = 0;
int elec = 1;

QString to_Send = "";
bool enter_Button_Clicked =false;

//for nemesys
double flowRate;
double volume;
bool opened = false;
bool dosing = false;

bool addRes = false;

bool mapCreated = false;    //From my understanding this signifies that the grid + reservoirs has been created

bool realTime = false;

QStringList ordCP;

bool csvFileRead = false;

bool deleteButtonchoice = false;

bool defaultNumbering = false;

int numberofButtons;

QList<int> Coordz;

bool coordsStored = false;      // may be temporary

int x;
int y;

//Used for Autogen path, color assignments
int* rcoord;
int* ccoord;
int track[4];
int size1;
int firstR,firstC;
boolean autoGen = false;


//Testing purposes
int aa =0;
int bb =0;
int cc =1000;
int dd =1000;
int ee = 2000;
int ff = 2000;


struct coordinates
{
    int x;
    int y;
}electrode_1,electrode_2;

DMFgui::DMFgui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DMFgui)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";

    ui->textEdit->setReadOnly(true); //not allowing user to change anything

    //for Nemesys
    ui->targetVolumeEdit->setReadOnly(true);
    ui->targetFlowRateEdit->setReadOnly(true);
    ui->unitsComboBox->addItem("nL");
    ui->unitsComboBox->addItem("μL");
    ui->unitsComboBox->addItem("mL");
    ui->unitsComboBox->addItem("L");
    ui->unitsComboBox->addItem("mm");

    ui->funitscomboBox->addItem("nL/s");
    ui->funitscomboBox->addItem("nL/min");
    ui->funitscomboBox->addItem("μL/s");
    ui->funitscomboBox->addItem("μL/min");
    ui->funitscomboBox->addItem("μL/h");
    ui->funitscomboBox->addItem("mL/min");
    ui->funitscomboBox->addItem("mL/h");
    ui->funitscomboBox->addItem("mm/s");

    ui->dosingUnitscomboBox->addItem("1");
    ui->dosingUnitscomboBox->addItem("2");
    ui->dosingUnitscomboBox->addItem("3");

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
        //arduino->open(QSerialPort::WriteOnly);
        arduino->open(QSerialPort::ReadWrite);


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

void DMFgui::closeEvent(QCloseEvent *event){

    if (opened){
        nemesys->closeConnection();
    }
    event -> accept();
}

void DMFgui::save_to_String(QString electrode_num)
{
    to_Send += electrode_num + ",";

    if (electrode_num=="Sequence" || electrode_num=="Split"|| electrode_num=="Mix")
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
    save_to_String("Mix");
}
void DMFgui::on_sequenceButton_clicked()
{
    save_to_String("1000");
}
void DMFgui::on_splitButton_clicked()
{
    save_to_String("Split");
}
void DMFgui::on_resetButton_clicked()
{
    to_Send = "";
    ui->textEdit->clear();
}

void DMFgui::autoGeneratePath(int rowI,int colI,int rowF, int colF, int path){

/*
    int currentrow = rowI;//y variable, getting called first
    int currentcol = colI;//x variable

    //for clearing purposes
    firstR = rowI;
    firstC = colI;

    while(currentrow != rowF || currentcol != colF)
    {
        bool activated = false;
        QStringList elecToCheck = findAvailableSpace(currentrow, currentcol).split(",");

        if (elecToCheck.contains("top",Qt::CaseSensitive) && !activated)
        {
            if((currentrow-1)>=rowF) //then you're getting closer
            {
                currentrow --;
                activate(currentrow,currentcol);
                activated = true;
            }
        }
        if(elecToCheck.contains("bottom",Qt::CaseSensitive)&& !activated)
        {
            if ((currentrow+1)<=rowF)
            {
                currentrow ++;
                activate(currentrow,currentcol);
                activated = true;
            }
        }
        if(elecToCheck.contains("right",Qt::CaseSensitive)&& !activated)
        {
            if((currentcol+1)<=colF)
            {
                currentcol++;
                activate(currentrow,currentcol);
                activated = true;
            }
        }
        if(elecToCheck.contains("left",Qt::CaseSensitive)&& !activated)
        {
            if((currentcol-1)>=colF)
            {
                currentcol--;
                activate(currentrow,currentcol);
                activated = true;
            }
        }
        activated = false;
    }
    dmf_array[rowI][colI].setStyleSheet("background-color:yellow; border-style: outset ;border-width: 2px; border-color: grey");
    dmf_array[rowF][colF].setStyleSheet("background-color:blue; border-style: outset ;border-width: 2px; border-color: grey");
*/
 /*
 *
 *  Kaleem's autogenerate path code.
 *
 *
     */
    QStringList tempL = to_Send.split(",");


    //Removes the two electrodes (start and end) from the Sending String
    //Works, but need to understand why certain steps are necessary
    tempL.removeLast();
    tempL.removeLast();
    tempL.removeLast();
    tempL.append("");
    to_Send = tempL.join(",");


    //creates local variables
    int trowI = rowI;
    int tcolI = colI;
    int trowF = rowF;
    int tcolF = colF;
    firstR = rowI;
    firstC = colI;
    //Determine number of electrodes to be turned on
    int size = abs(trowI-trowF)+abs(tcolI-tcolF);
    size1 =size;

    dmf_array[rowI][colI].setStyleSheet("background-color:yellow; border-style: outset ;border-width: 2px; border-color: grey");     //starting electrode becomes yellow
   // ui->textEdit->insertPlainText("\ncolI: " +QString::number(colI));
   // ui->textEdit->insertPlainText("\nrowI: " +QString::number(rowI));
    //Create an array containing xcoordinates and ycoordinates
    int *xcoord = new int [size];
    int *ycoord = new int [size];
    //Determines number of rows and columns needed to travel
    int rows = abs(trowI-trowF);
    int cols = abs(tcolI-tcolF);
  //Determines which direction the electrode is traveling (northwest,northeast,southwest,southeast
    bool rowCond = (trowF-trowI)>=0;
    bool colCond = (tcolF-tcolI)>=0;
    int a;
  //Southeast
  if (rowCond ==1 && colCond ==1){
       if (path == 1){
          for(a =0; a<cols; a++){
              xcoord[a] = ++tcolI;
              ycoord[a] = trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = tcolF;
              ycoord[a] = ++trowI;
          }
      }
      else if (path == 2){
          for(a = 0; a<rows; a++){
              xcoord[a] = tcolI;
              ycoord[a] = ++trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = ++tcolI;
              ycoord[a] = trowF;
          }
      }
  }
//Southwest
  else if (rowCond ==1 && colCond ==0){
      if (path == 1){
          for(a=0; a<cols; a++){
              xcoord[a] = --tcolI;
              ycoord[a] = trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = tcolF;
              ycoord[a] = ++trowI;
          }
      }
      else if (path == 2){
          for(a = 0; a<rows; a++){
              xcoord[a] = tcolI;
              ycoord[a] = ++trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = --tcolI;
              ycoord[a] = trowF;
          }
      }
  }
  //Northwest
  else if (rowCond ==0 && colCond ==0){
      if (path == 1){
          for(a = 0; a<cols; a++){
              xcoord[a] = --tcolI;
              ycoord[a] = trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = tcolF;
              ycoord[a] = --trowI;
          }
      }
      else if (path == 2){
          for(a = 0; a<rows; a++){
              xcoord[a] = tcolI;
              ycoord[a] = --trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = --tcolI;
              ycoord[a] = trowF;
          }
      }
  }
  //Northeast
  else if (rowCond ==0 && colCond ==1){
      if (path == 1){
          for(a = 0; a<cols; a++){
              xcoord[a] = ++tcolI;
              ycoord[a] = trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = tcolF;
              ycoord[a] = --trowI;
          }
      }
      else if (path == 2){
          for(a = 0; a<rows; a++){
              xcoord[a] = tcolI;
              ycoord[a] = --trowI;
          }
          for(a;a<size;a++){
              xcoord[a] = ++tcolI;
              ycoord[a] = trowF;
          }
      }
  }
  //Create array of electrodes to be turned on and store in a String
  QString *retMap  = new QString [size];
  for (int m =0; m<size;m++){
      retMap[m] = dmf_array[ycoord[m]][xcoord[m]].text();
      //Set electodes to be activated: green
      dmf_array[ycoord[m]][xcoord[m]].setStyleSheet("background-color:green; border-style: outset ;border-width: 2px; border-color: grey");
      save_to_String(retMap[m]);
  }
  //Set final electrode to blue
  dmf_array[rowF][colF].setStyleSheet("background-color:blue; border-style: outset ;border-width: 2px; border-color: grey");
 // ui->textEdit->insertPlainText("\ncolF: " +QString::number(colF));
 // ui->textEdit->insertPlainText("\nrowF: " +QString::number(rowF));
  //Pointers used to turn colors off
  rcoord = ycoord;
  ccoord = xcoord;
  autoGen = true;
  ui->textEdit->setPlainText(to_Send);

  //Used to set the text cursor to the end of the text
  //Error with autogen path, text cursor was set to the beginning, will debug if tjere is time
  QTextCursor textEditCursor = ui->textEdit->textCursor();
  int textlength = ui->textEdit->toPlainText().length();
  textEditCursor.setPosition(textlength,QTextCursor::MoveAnchor);
  ui->textEdit->setTextCursor(textEditCursor);


  // For testing purposes only
  // ui->textEdit->setFocus();
   /* *
    *
    *
    * end of Kaleem's autogenerate path code
  */
}

//finds which electrodes are available next to the current electrode
QString DMFgui::findAvailableSpace(int y, int x){
    QString availElec = "";

    //check on top of the current electrode
    if (dmf_array[y-1][x].text()!="")
    {
        availElec += "top,";
    }
    //check bottom of current electrode
    if (dmf_array[y+1][x].text()!="")
    {
        availElec += "bottom,";
    }
    //check to the left of current electrode
    if (dmf_array[y][x-1].text()!="")
    {
        availElec += "left,";
    }
    //check to the right of current electrode
    if (dmf_array[y][x+1].text()!="")
    {
        availElec += "right,";
    }
    return availElec;
}
void DMFgui::activate(int y, int x)
{
    save_to_String(dmf_array[y][x].text());
    dmf_array[y][x].setStyleSheet("background-color:green; border-style: outset ;border-width: 2px; border-color: grey");
}

// ** next step: allowing user to modify autogenerate path **
void DMFgui::ClearColor(){
    if(autoGen){
        dmf_array[firstR][firstC].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
        for (int q=0;q<size1;q++){
            for(int r=0;r<size1;r++){
            dmf_array[rcoord[q]][ccoord[r]].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
            }
        }
    }
    autoGen = false;
}
void DMFgui::on_exitButton_clicked()
{
    QByteArray readData = arduino->readAll();
    ui->textEdit->insertPlainText(readData);

    //QApplication::quit();
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
        //ui->textEdit->insertPlainText(to_Send);
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
                        //dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: yellow");
                    }
                    else
                    {
                        numberingcount++;



                        //inserting values into the array
                       //dmf_array[i][j].setText(ordCP.at(numberingcount-1));//QString::number(numberingcount));

                        //dmf_array[i][j].setText(QString::number(numberingcount));
                        dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                        gridLayout->addWidget(&dmf_array[i][j],i,j);
                        mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                        //mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+ordCP.at(numberingcount-1));//QString::number(numberingcount));
                        //mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(numberingcount));
                        if(defaultNumbering){
                            dmf_array[i][j].setText(QString::number(numberingcount));
                            mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(numberingcount));

                        }
                        else{
                            dmf_array[i][j].setText("_");
                            mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+"_");
                        }
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
                bool number = false;
                while (!number)
                {
                   QMessageBox::warning(this,tr("Not a number"), tr("This is not a number, try again"));
                   text = QInputDialog::getText(this,tr("Next Step"),tr("How many reservoirs do you want?"),QLineEdit::Normal,QDir::home().dirName(),&ok);
                   if (text.toInt() == 0)
                   {
                      number = false;
                   }
                   else
                   {
                       number = true;
                   }
                }
            }

            if(ok&&!text.isEmpty())
            {
                //number of reservoirs
                resnum = text.toInt();
                ui->textEdit->insertPlainText("please select " + text + " reservoirs");

                addRes = true;
            }
            enter_Button_Clicked = true;
            mapCreated = true; // may be a repition of enter_button_clicked, need to check with Meko
        }
    }
    else
    {
        if (enter_Button_Clicked)
        {
            QMessageBox::warning(this,tr("Invalid"), tr("You can't click on this button again"));
        }
    }
}

/*--------------------------
 * Allows user to turn on electrodes instantly after pressing the button
 * It only allows for sequencing
 * --------------------------*/
void DMFgui::on_realTimeBox_clicked(bool checked)
{
    if (checked){
        realTime = true;
    }
    else
        realTime = false;
}

/*---------
 * If delete button is pressed, you can  buttons
 * It will be used to create layouts that can also be saved later
 * ------*/

void DMFgui::on_DeleteBox_clicked(bool checked)
{
    if (checked){
        deleteButtonchoice=true;
    }
    else {
        deleteButtonchoice = false;
    }
}


//When an electrode is clicked
void DMFgui::buttonClicked(QString text)
{
    QStringList electrodeList;
    electrodeList = text.split(",");
    if(!deleteButtonchoice){
        ClearColor();
        //QStringList electrodeList;
        //electrodeList = text.split(",");
        if (elec ==1)
        {
            electrode_1.x = electrodeList.value(1).toInt();
            electrode_1.y = electrodeList.value(0).toInt();

    //        ui->textEdit->insertPlainText("\n electrode_1");
    //        ui->textEdit->insertPlainText("\n electrode: " +electrodeList.value(2));
    //        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_1.x));
    //        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_1.y));

            elec ++; //go to electrode_2 next time a button is pressed
        }
        else if(elec==2)
        {
            electrode_2.x = electrodeList.value(1).toInt();
            electrode_2.y = electrodeList.value(0).toInt();

    //        ui->textEdit->insertPlainText("\n electrode_2");
    //        ui->textEdit->insertPlainText("\n electrode: " + electrodeList.value(2));
    //        ui->textEdit->insertPlainText("\n x coordinate: " +QString::number(electrode_2.x));
    //        ui->textEdit->insertPlainText("\n y coordinate: " +QString::number(electrode_2.y));

            elec --; //go to electrode_1 next time a button is pressed
        }

        //if addReservoir was called
        if (addRes)
        {
            if (added<resnum)
            {
               if (add_reservoir(newcolumn,newrow,resnum))
               {
                   added++;
               }
            }
            else
            {
                addRes = false;
                QMessageBox::warning(this,tr("Next Step"), tr("You can start generating paths"));
            }
        }

        else
        {
            if(realTime){
                if (arduino->isWritable()){
                    QString a = "1000," + electrodeList.value(2);
                    const char * dat = a.toStdString().c_str(); //= ("1000," + electrodeList.value(2)).toStdString().c_str();

                    arduino->write(dat);

                }
            }
            else{
            //ui->textEdit->insertPlainText("\n called");
                save_to_String(electrodeList.value(2));
            }

        }
    }
    else{

      //Deletes Buttons
        ui->textEdit->setPlainText("");
        int* retIndex = returnIndex(dmf_array,newrow,newcolumn,electrodeList.value(2).toInt());
        int row1 = retIndex[0];
        int col1 = retIndex[1];

        ui->textEdit->setPlainText("row: " + QString::number(row1) + ", col: " + QString::number(col1));
        dmf_array[row1][col1].setText("");//QString::number(numberingcount));
        dmf_array[row1][col1].setStyleSheet("background-color: white;border-color:none;border-style: outset ;");
        dmf_array[row1][col1].setEnabled(false);



    }
}

bool DMFgui::add_reservoir(int column, int row, int resnum)
{
    int * coords = getRecent_Coordinates();

    //getting the most recent coordinates of pressed electrodes
    int x_coord = coords[2];
    int y_coord = coords[3];

    QPushButton *reservoir = new QPushButton;
    reservoir->setText("res");
    reservoir->setEnabled(false);//cannot press this button
    reservoir->setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");

    int caseSwitch;

    //the cases work fine. They're accurate
    if ((x_coord==2&&y_coord==2)||(x_coord==2&&y_coord==(row-3))||(x_coord==(column-3)&&y_coord==(row-3))||(x_coord==(column-3)&&y_coord==2))
    {
        caseSwitch = 2; //corner
    }
    else if (x_coord==2||x_coord==(column-3)||y_coord==2||y_coord==(row-3))
    {
        caseSwitch = 1; //not a corner
    }
    else
    {
        caseSwitch = 0; //not a valid reservoir
    }

    switch (caseSwitch)
    {
        case 2:
            numberingcount++;

            //ask user if they want a reservoir on the left or on the right
            //make a separate case for each 4 corners
            if (x_coord==2&&y_coord==2)
            {
                corner = 1; //"top-left"
            }
            else if (y_coord==2&&x_coord==(row-3))
            {
                corner = 4;//"bottom-left"
            }
            else if (y_coord==(column-3)&&x_coord==(row-3))
            {
                corner = 3;//"bottom-right"
            }
            else if (y_coord==(column-3)&&x_coord==2)
            {
                corner = 2;//"top-right"
            }

            location = openNewWindow(corner);

            if (location=="top")
            {
                gridLayout->addWidget(reservoir,0,y_coord);
                setMapping(y_coord,1);
                return true;
                break;
            }
            if (location=="left")
            {
                gridLayout->addWidget(reservoir,x_coord,0);
                setMapping(1,x_coord);
                return true;
                break;
            }
            if (location=="right")
            {
                gridLayout->addWidget(reservoir,x_coord,row-1);
                setMapping(row-2,x_coord);
                return true;
                break;
            }
            if (location=="bottom")
            {
                gridLayout->addWidget(reservoir,column-1,y_coord);
                setMapping(y_coord,column-2);
                return true;
                break;
            }
            return true;
            break;
        case 1:
            //add new buttons depending on where the button clicked is (reservoir is hatched)
                numberingcount++;

                //set booleans so that you don't get to click to same place twice
                if (y_coord==2)//left column
                {
                    gridLayout->addWidget(reservoir,x_coord,0);
                    setMapping(1,x_coord);
                    return true;
                    break;
                }
                else if (y_coord==(column-3)) //right column
                {
                    gridLayout->addWidget(reservoir,x_coord,column-1);
                    setMapping(column-2,x_coord);
                    return true;
                    break;
                }
                else if (x_coord==2)
                {
                    gridLayout->addWidget(reservoir,0,y_coord);
                    setMapping(y_coord,1);
                    return true;
                    break;
                }
                else if(x_coord==(row-3))
                {
                    gridLayout->addWidget(reservoir,row-1,y_coord);
                    setMapping(y_coord,row-2);
                    return true;
                    break;
                }
        case 0:
            QMessageBox::warning(this,tr("Invalid"), tr("You can't put a reservoir here"));
            return false;
            break;
    }
}

void DMFgui::setMapping(int x, int y)
{
    dmf_array[y][x].setText(QString::number(numberingcount));
    dmf_array[y][x].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
    gridLayout->addWidget(&dmf_array[y][x],y,x);
    mapper->connect(&dmf_array[y][x],SIGNAL(clicked()),mapper,SLOT(map()));
    mapper->setMapping(&dmf_array[y][x],QString::number(y)+","+QString::number(x)+","+QString::number(numberingcount));
}

int * DMFgui::getRecent_Coordinates(){
    int recentCoord[4];
    if (elec==1)
    {
        recentCoord[0] = electrode_1.y;
        recentCoord[2] = electrode_2.y;
        recentCoord[1] = electrode_1.x;
        recentCoord[3] = electrode_2.x;
    }
    else if (elec==2)
    {
        recentCoord[0] = electrode_2.y;
        recentCoord[2] = electrode_1.y;
        recentCoord[1] = electrode_2.x;
        recentCoord[3] = electrode_1.x;
    }
    return recentCoord;
}


void DMFgui::updateDMF(QString to_Send)
{


    //Split the String into smaller string stored in QString array
    QStringList sep = to_Send.split(",");
    int numVals = 5;
    double nVal = 5;
    int actLength = sep.length()-1;
    double a = ceil(actLength/nVal);
    int rem = actLength%numVals;
    ui->textEdit->insertPlainText("rem: " + QString::number(rem)+"\n");
    ui->textEdit->insertPlainText("length: " + QString::number(actLength)+"\n");
    ui->textEdit->insertPlainText("ceil: " + QString::number(a)+"\n");
    int b = a;

    ui->textEdit->insertPlainText("convceil: " + QString::number(b)+"\n");
    QString sep20 [b];
    int q;
    if(rem==0){
        q=b;
    }
    else
        q=b-1;
    for(int i=0;i<q;i++){             //Create strings up until the very last one which may contain less than numVals electrodes
        for(int j=(i*numVals);j<(i*numVals)+(numVals-1);j++){
            sep20[i]=sep20[i].append(sep.at(j)+",");
        }
        sep20[i]=sep20[i].append(sep.at((i*numVals)+(numVals-1)));
    }
    if(rem!=0){
    for(int k=((b-1)*numVals);k<((b-1)*numVals)+(rem-1);k++){
        sep20[b-1]=sep20[b-1].append(sep.at(k)+",");
    }
    sep20[b-1]=sep20[b-1].append(sep.at((b-1)*(numVals)+(rem-1)));
    }//ui->textEdit->setPlainText("");
    for (int k=0;k<b;k++){
        ui->textEdit->insertPlainText(sep20[k] + "\n");
    }
    if (arduino->isWritable())
    {


        /*if (readData==v){
            arduino->write(sep20[0].toStdString().c_str());
        }
        else
            ui->textEdit->insertHtml("\n Didn't work");
        */
        QByteArray readData = arduino->readAll();
        QByteArray v = "2";
        QString s_data = readData;
        ui->textEdit->insertPlainText("data: " + s_data);

        //arduino->write(sep20[0].toStdString().c_str());

        for(int i = 0; i<b;i++){
            arduino->write(sep20[i].toStdString().c_str());
            while(readData!=v){
                readData = arduino->readAll();
                s_data = readData;
                ui->textEdit->insertPlainText(s_data);
                qApp->processEvents();
            }
            readData = arduino->readAll();
             ui->textEdit->insertPlainText("\ni: " + QString::number(i));
            //Delay may be needed
        }



        /*arduino->write(to_Send.toStdString().c_str());
        qApp->processEvents();


       QByteArray readData = arduino->readAll();

       QString s_data = readData;
       ui->textEdit->insertPlainText(s_data);

       //Exits loop once it's reached the end point
       //At this point you can send more data
       while(s_data!="reached this point"){
           readData = arduino->readAll();
           s_data = readData;
           ui->textEdit->insertPlainText(s_data);
           qApp->processEvents();
       }*/

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

    if (corner == 1)//"top-left"
    {
        dialog->choice("topLeft");
        dialog->exec();
        return dialog->saved;
    }
    else if (corner == 2)//"top-right"
    {
        dialog->choice("topRight");
        dialog->exec();
        return dialog->saved;
    }
    else if (corner == 3)//"bottom-right"
    {
        dialog->choice("bottomRight");
        dialog->exec();
        return dialog->saved;
    }
    else if(corner == 4)//"bottom-left"
    {
        dialog->choice("bottomLeft");
        dialog->exec();
        return dialog->saved;
    }
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

    funcgen = new funcGen();

    funcgen->send_voltage(to_Send);

    }

void DMFgui::on_autogen_Button_clicked()
{
    // Four inputs
    int *z = getRecent_Coordinates();

       int a1 = z[0];
       int a2 = z[1];
       int a3 = z[2];
       int a4 = z[3];
        //Path 1 is set as a default
        autoGeneratePath(a1,a2,a3,a4,1);
}

void DMFgui::on_targetVolume_clicked()
{
    ui->targetFlowRateEdit->setReadOnly(true);
    //allowing for the value to be changed
    ui->targetVolumeEdit->setReadOnly(false);
    volume = ui->targetVolumeEdit->text().toDouble();
}

void DMFgui::on_targetFlow_clicked()
{
    ui->targetVolumeEdit->setReadOnly(true);
    //allowing for the value to be changed
    ui->targetFlowRateEdit->setReadOnly(false);
    flowRate = ui->targetFlowRateEdit->text().toDouble();
}

void DMFgui::nemesysNotOpenedErrorMessage()
{
    QMessageBox::warning(this,tr("NemeSYS not Opened"), tr("Please open NemeSYS first"));
}

void DMFgui::nemesysAlreadyOpenedMessage()
{
    QMessageBox::warning(this,tr("NemeSYS Opened"), tr("NemeSYS is already opened"));
}


void DMFgui::nemesysDosingMessage()
{
    QMessageBox::warning(this,tr("NemeSYS is Dosing"), tr("Please wait until Dosing Unit is finished Dosing/Calibrating"));
}


//May be unnecessary, to be confirmed
void DMFgui::nemesysCalibrateMessage()
{
    QMessageBox::warning(this,tr("NemeSYS is Calibrating"), tr("Please wait until Dosing Unit is finished Calibrating"));
}

void DMFgui::on_OpenButton_clicked()
{
    nemesys = new Nemesys();

    if (!opened)
    {
        nemesys->openConnection();
    }
    else
    {
        nemesysAlreadyOpenedMessage();
    }

    opened = true;
}

void DMFgui::on_CloseButton_clicked()
{
    if (opened)
    {
        nemesys->closeConnection();
        opened = false;
    }
    else
    {
        nemesysNotOpenedErrorMessage();
    }
}


//For now, put a positive volume and negative flow
void DMFgui::on_doseButton_clicked()
{
    //Open and not dosing or calibrating
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

        double volume = ui->targetVolumeEdit->text().toDouble();

        double flow = ui->targetFlowRateEdit->text().toDouble();

        unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
        nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->DoseVolume(dosingUnit, volume,flow);
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0)
    {
        nemesysDosingMessage();
    }

    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
    else if (!opened)
    {
        nemesysNotOpenedErrorMessage();
    }
}

//For now put a positive flow value
void DMFgui::on_emptyButton_clicked()
{
    //Open and not dosing or calibrating
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)    // && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

        double flow = ui->targetFlowRateEdit->text().toDouble();

        unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
        nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->EmptySyringe(dosingUnit, flow);
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0)
    {
         nemesysDosingMessage();
    }

    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
     else if (!opened)
    {
         nemesysNotOpenedErrorMessage();
    }
}


//For now, ensure that you have a negative flow value
void DMFgui::on_refillButton_clicked()
{
    //Open and not dosing or calibrating
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

        double flow = ui->targetFlowRateEdit->text().toDouble();

        unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
        nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

        unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
        nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

        nemesys->RefillSyringe(dosingUnit, flow);
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0)
    {
         nemesysDosingMessage();
    }

    //May be unnecessary, to be confirmed
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }
     else if (!opened)
    {
         nemesysNotOpenedErrorMessage();
    }
}

void DMFgui::on_CalibrateButton_clicked()
{
    //Open and not dosing
    if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 1)// && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()) == 1))
    {

        //Warning Message to make sure no syringes are placed
        QMessageBox calibrateBox;
        calibrateBox.setWindowTitle("Calibration Warning");
        calibrateBox.setText("Please ensure that no syringes are placed in the dosing unit.\nExecuting the calibration move with a syringe fitted on the device may cause damage to the syringe.\n\nAre you ready?");
        QAbstractButton* pButtonYes = calibrateBox.addButton(tr("Yes"), QMessageBox::YesRole);
        calibrateBox.addButton(tr("No"), QMessageBox::NoRole);

        calibrateBox.exec();

        //Execute calibration if user is ready
        if (calibrateBox.clickedButton()==pButtonYes) {
            unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());

            unsigned char volumeUnit = (unsigned char)(ui->unitsComboBox->currentIndex());
            nemesys->setActiveVolumeUnit(dosingUnit, volumeUnit);

            unsigned char flowUnit = (unsigned char)(ui->funitscomboBox->currentIndex());
            nemesys->setActiveFlowUnit(dosingUnit, flowUnit);

            nemesys->CalibrateUnit(dosingUnit);
            nemesys->CheckCalibrateStatus(dosingUnit);
        }
    }
    else if (opened && nemesys->CheckDosingStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysDosingMessage();

    }
    else if (opened && nemesys->CheckCalibrateStatus((unsigned char)(ui->dosingUnitscomboBox->currentIndex()))== 0){
        nemesysCalibrateMessage();
    }

    else if (!opened)
    {
        nemesysNotOpenedErrorMessage();
    }

}

void DMFgui::on_StopButton_clicked()
{
    if(!opened)
    {
        nemesysNotOpenedErrorMessage();
    }
    else
    {
        unsigned char dosingUnit = (unsigned char)(ui->dosingUnitscomboBox->currentIndex());
        nemesys->StopUnit(dosingUnit);
    }
}


void DMFgui::on_MButton1_clicked()
{
    aa++;
    bb++;
    //double actualFlow = nemesys->getActualFlowRate(0) ;
    //double actualSyringe = nemesys->getActualSyringeLevel(0);

    ui->TargetMonitorFlowRate->setText(QString::number(aa));
    ui->TargetMonitorSyringeLevel->setText(QString::number(bb));
}


/*
   double sminL, smaxL;
   double* sL = nemesys->getSyringeLevels(0, sminL,smaxL);
   ui->TargetMonitorFlowRate->setText(QString::number(sL[0]));
   ui->TargetMonitorSyringeLevel->setText(QString::number(sL[1]));

 */

void DMFgui::on_MonitorBox1_toggled(bool checked)
{
    if (checked)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton1->setDown(true);
        ui->MButton1->setAutoRepeat(true);
        ui->MButton1->setAutoRepeatDelay(rate);
        ui->MButton1->setAutoRepeatInterval(rate);
    }
    else
    {
        ui->MButton1->setAutoRepeat(false);
        ui->MButton1->setDown(false);
    }
}

void DMFgui::on_MonitorBox2_toggled(bool checked)
{
    if (checked)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton2->setDown(true);
        ui->MButton2->setAutoRepeat(true);
        ui->MButton2->setAutoRepeatDelay(rate);
        ui->MButton2->setAutoRepeatInterval(rate);
    }
    else
    {
        ui->MButton2->setAutoRepeat(false);
        ui->MButton2->setDown(false);
    }
}

void DMFgui::on_MButton2_clicked()
{
    cc++;
    dd++;


    ui->TargetMonitorFlowRate2->setText(QString::number(cc));
    ui->TargetMonitorSyringeLevel2->setText(QString::number(dd));
}

void DMFgui::on_MonitorBox3_toggled(bool checked)
{
    if (checked)
    {
        int rate = ui->targetRefreshRateEdit->text().toInt();
        ui->MButton3->setDown(true);
        ui->MButton3->setAutoRepeat(true);
        ui->MButton3->setAutoRepeatDelay(rate);
        ui->MButton3->setAutoRepeatInterval(rate);
    }
    else
    {
        ui->MButton3->setAutoRepeat(false);
        ui->MButton3->setDown(false);
    }
}

void DMFgui::on_MButton3_clicked()
{
    ee++;
    ff++;
    ui->TargetMonitorFlowRate3->setText(QString::number(ee));
    ui->TargetMonitorSyringeLevel3->setText(QString::number(ff));
}



/*---------------------------------------
 *
 *
 * Functions for Writing/Reading Sequence files
 *
 * ---------------------------------------*/

/*------------
 * Write the sequence that is in the serial monitor to a text file
 * ----------*/

void DMFgui::on_Send1Button_clicked()
{
    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else{
        //C:/Users/kaleem/Summer_2016/Steve Shih Project/GitHub/dmf3/DMF-master (2)/DMF-master/dmf32/ex.txt
        //Ensure that the path is correct, in this case, it places it in the build folder

        QFile file ("ex3.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
    }
}

/*------------
 * Read the sequence that is in the text file and send display it on the serial monitor
 * "line" contains the sequence
 * In this case, the text file is stored locally in the Build folder
 * ----------*/

void DMFgui::on_ReadButton_clicked()
{
    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else{
        QFile file ("ex3.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString line;

        //QString is suppose to append the next line after the previous one, however this isn't working exactly as expected, instead the line is inserted before
        while (!in.atEnd()) {
             line = in.readLine().insert(0,line);
        }
        ui->textEdit->setPlainText(line);
    }
 }


/*------------
 * Get the Index values (row and column) from an electrode
 * Returns it in a integer pointer
 * ----------*/

int* DMFgui::returnIndex(QPushButton** arr,int sizerow, int sizecol, int seek){
    int val [2];
    int err [2] = {-1,-1};
    for (int i=0;i<sizerow;i++){
        for (int j = 0; j<sizecol; j++){
            if (arr[i][j].text().toInt()==seek){
                val[0]=i;
                val[1]=j;
                return val;
            }
        }
    }
    return err;
}

/*------------
 * Writebox, if enabled -> Allows User to modify the Serial Monitor
 * Can be used to manually change the sequence without having to press down on a button
 * ----------*/
void DMFgui::on_WritetoTextBox_clicked(bool checked)
{
    if (checked){
        ui->textEdit->setReadOnly(false);
    }
    else
        ui->textEdit->setReadOnly(true);
}


/*------------
* Preview the sequence in the Serial Monitor
* Each electrode turns on one after another
* Color is set depending on the type of sequencing (sequence, mix, split,etc.)
* Preview delay is set in seconds by the user
* ----------*/

void DMFgui::on_PreviewButton_clicked()
{

    if (!mapCreated){
        mapNotCreatedErrorMessage();
    }

    else {
        QString previewElectrodes = ui->textEdit->toPlainText();
        QStringList electrodeList = previewElectrodes.split(",");
        double previewDelay = ui->PreviewSpeedEdit->text().toDouble()*1000;
        QString previewColor;
        ui->textEdit->clear();
        for (int a = 0; a<electrodeList.size(); a++){
            if(electrodeList.at(a)=="1000"){
                previewColor = "red";
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);
            }
            else if (electrodeList.at(a)=="Mix"){
                previewColor = "orange";
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);
            }

            //Temporary solution, it only splits two electrodes, need to come up with a better solution
            else if (electrodeList.at(a)=="Split"){
                previewColor = "blue";
                int b = a+1;
                int c = a+2;
                ui->textEdit->insertPlainText(electrodeList.at(a)+",");
                qApp->processEvents();
                Sleep(previewDelay);

                ui->textEdit->insertPlainText(electrodeList.at(b)+",");
                ui->textEdit->insertPlainText(electrodeList.at(c)+",");

                //Get index of the element one positions away from 1001
                int* retIndex_1 = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(b).toInt());
                int row1 = retIndex_1[0];
                int col1 = retIndex_1[1];

                //Get index of the element two positions away from 1001
                int* retIndex_2 = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(c).toInt());
                int row2 = retIndex_2[0];
                int col2 = retIndex_2[1];

                dmf_array[row1][col1].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                dmf_array[row2][col2].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
                Sleep(previewDelay);
                dmf_array[row1][col1].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                dmf_array[row2][col2].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
                a+=2;
            }
            else{
                int* retIndex = returnIndex(dmf_array,newrow,newcolumn,electrodeList.at(a).toInt());
                int row1 = retIndex[0];
                int col1 = retIndex[1];

                ui->textEdit->insertPlainText(electrodeList.at(a)+",");

                dmf_array[row1][col1].setStyleSheet("background-color:"+previewColor+"; border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
                Sleep(previewDelay);
                dmf_array[row1][col1].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                qApp->processEvents();
            }
        }
    }
}

/*-----------------------------
 *
 * Functions:
 * Create a New Map Layout
 * Saving a Map Layout
 * Loading a Map Layout
 * Reading a Txt file --> represents the contact pad information
 * Filling the buttons with the contact pad information based on text file
 *
 * ------------------------------*/


/*------
 * New Map Layout, allow access to row and column text edit
 * -----*/
void DMFgui::on_newLayoutButton_clicked()
{
    openNewLayoutWindow();
    //ui->rowEdit->setEnabled(true);
    //ui->columnEdit->setEnabled(true);
}

/*--------------
 * Steps for getting contact pad informatoin:
 * 1.export the contact pad information into a .asc file
 * 2.Save the file as a text delimited file, a .txt file, in the build folder (for now)
 * This Function --> 3. read the text file and intrepret information
 *
 * Steps for this function:
 * A- READ TXT FILE CONTAINING ELECTRODE + CONTACT PAD INFORMATION
 * B- FILTERS OUT LINES THAT DON'T CONTAIN NECESSARY INFO
 * C- SORT THE INFORMATION IN ASCENDING ORDER (FROM TOP-LEFT TO BOTTOM-RIGHT)
 * ---------------*/



void DMFgui::on_readCSVButton_clicked()
{
    if(!csvFileRead){
        QFile file ("dmfdesign1.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString line;
        QStringList a;
        int count =0;

        //READ TXT FILE CONTAINING ELECTRODE + CONTACT PAD INFORMATION
        //FILTERS OUT LINES THAT DON'T CONTAIN NECESSARY INFO
        //STORE LINES THAT CONTAIN T1 ==> THIS REPRESENTS RESDES,

        while (!in.atEnd()) {
             line = in.readLine();
             if(line.contains("T1.", Qt::CaseInsensitive)){
                 a.append(line);
                 count++;
             }
        }

        //SORTING ENSURES THAT ALL OF THE ELECTRODES ARE TO THE LEFT, THIS IS TO ENSURE THAT ASCENDING SORTING CAN BE DONE
        //ADDs "0"s if needed, this is to ensure that sorting is done properly
        //CALCULATES THE NUMBER of "0"s to add
         //CRUDE METHOD FOR NOW: ADD "0" depending on how many need to be added)

        QStringList b;

        for(int i=0;i<count;i++){
            b = a.at(i).split(" ");
            sort(b.begin(),b.end());
            if(b.at(5).length()<6){
                int a = 6-b.at(5).length();
                QStringList c = b.at(5).split(".");
                QString d;
                if(a==1){
                    d="0";
                }
                else if(a==2){
                    d="00";
                }
                else if(a==3){
                    d="000";
                }
                b.replace(5,(c.at(0)+"."+d+c.at(1)));               //CONCATENATES ADDED "0"s
            }
            a.replace(i,(b.at(5)+" "+b.at(6)));                     //CONCATENATES CONTACT PADS AND ELECTRODES BACK TOGETHER
        }
        sort(a.begin(),a.end());                                    //SORTS INTO ASCENDING ORDER LIST OF ELECTRODES WITH THEIR RESPECTIVE CONTACT PADS
        QStringList tempCP;
        QRegExp rx("(\\ |\\.)");                                    //RegEx for ' ' or ','
        for(int k = 0;k<count;k++){
            tempCP = a.at(k).split(rx);
            ordCP.append(tempCP.at(3));
        }
        for(int w=0;w<ordCP.length();w++){
            ui->textEdit->insertPlainText(ordCP.at(w)+"\n");        //ONLY DISPLAYS RESPECTIVE CONTACT PADS
        }
        ui->textEdit->insertPlainText("LENGTH: "+ QString::number(ordCP.length()));
        csvFileRead=true;
    }
    else
    {
        QMessageBox::warning(this,tr("csvFile"), tr("This file has already been read"));
    }
}


/*-------------
 * Fills the buttons with the contact pad information after the map has been created and the csv file has been read
 * -----------*/

void DMFgui::on_fillTextButton_clicked()
{
    if(csvFileRead){// && ordCP.length()==numberofButtons){
        int z=0;
        for (int i=0;i<newrow;i++)
        {
            for (int j=0;j<newcolumn;j++)
            {
                if(dmf_array[i][j].text()!=""){
                    dmf_array[i][j].setText(ordCP.at(z));
                    mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                    mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+ordCP.at(z));     // numberingcount-1));//QString::number(numberingcount));
                    z++;
                }
            }
        }      
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(buttonClicked(QString)));
    }
    else if(!csvFileRead)
    {
        csvFileNotRead();
    }
    else {
        QMessageBox::warning(this,tr("Buttons"), tr("The number of buttons and number of contact pads are different"));
    }
}

/*-----------
 * Loads the map layout information from the text file
 * For now, the text file is located in the build folder
 * ---------*/

void DMFgui::on_loadLayoutButton_clicked()
{
    if(!mapCreated){
        QFile file ("savedLayout.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString line;
        QStringList mazeInfo;
        while (!in.atEnd()) {
             mazeInfo.append(in.readLine());
        }

        QList <int> CreateMapInfoRow;
        QList <int> CreateMapInfoCol;

        QStringList tempMap;
        for(int a=0;a<mazeInfo.length();a++){
            tempMap = mazeInfo.at(a).split(",");
            CreateMapInfoRow.append(tempMap.at(0).toInt());
            CreateMapInfoCol.append(tempMap.at(1).toInt());
        }

        /*-------------------------
         * Creating Map
         * ------------------------*/

        gridLayout = new QGridLayout;
        mapper = new QSignalMapper;

        gridLayout->setHorizontalSpacing(0);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setSpacing(0);


        int maprow = CreateMapInfoRow.at(0);
        int mapcol = CreateMapInfoCol.at(0);

        /*-----------------------------------------
         * Globally change the newrow and newcolumn value
         *
         * --------------------------------------*/
        newrow = maprow;
        newcolumn = mapcol;


        dmf_array = new QPushButton*[maprow];
        for(int i=0;i<maprow;i++){
            dmf_array[i]=new QPushButton[mapcol];
        }

        QLabel *empty = new QLabel(this);

        int s=1;
        for (int i=0;i<maprow;i++)
        {
            for (int j=0;j<mapcol;j++)
            {
                //If the index corresponds to one that is in our button information, create the button at that location
                if (s<CreateMapInfoCol.length() && i==CreateMapInfoRow.at(s) && j==CreateMapInfoCol.at(s))
                {
                    //The "_" is used for now, but anything can be used as a means to differntiate from empty boxes which have text = ""
                    dmf_array[i][j].setText("_");
                    dmf_array[i][j].setStyleSheet("border-style: outset ;border-width: 2px; border-color: grey");
                    gridLayout->addWidget(&dmf_array[i][j],i,j);
                    s++;
                }
                //If not, create an empty box
                else
                {
                    gridLayout->addWidget(empty,i,j);
                }
            }
        }
        //numberofButtons = CreateMapInfoCol.length()-1;
        //Set the layout
        ui->graphicsView->setLayout(gridLayout);
        mapCreated=true;
    }
    else{
        QMessageBox::warning(this,tr("Map Creation"), tr("Map has already been created yet \nYou can't load at this time."));
    }
}

/*-----------
 * Save Layout information in a list
 * Info to be stored:
 * The map size (row and column)
 * The buttons to be created (row and column)
 * ---------*/
void DMFgui::on_saveLayoutButton_clicked()
{
    if(!mapCreated){
        mapNotCreatedErrorMessage();
    }
    else if(!coordsStored){
        int z=0;
        Coordz.append(newrow);
        Coordz.append(newcolumn);
        ui->textEdit->setPlainText("");

        ui->textEdit->insertPlainText( QString::number(Coordz.at(0)) + "," + QString::number(Coordz.at(1))+ "\n");
        for (int i=0;i<newrow;i++)
        {
            for (int j=0;j<newcolumn;j++)
            {
                if(dmf_array[i][j].text()!=""){
                    z++;
                    Coordz.append(i);
                    Coordz.append(j);
                }
            }
        }
        for(int p=2;p<Coordz.length();p++){
            ui->textEdit->insertPlainText(QString::number(Coordz.at(p)) + "," + QString::number(Coordz.at(p+1))+ "\n");
            p++;
        }
        QFile file ("savedLayout.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();             //**There may be a better way to send the info than through the textEdit
        coordsStored=true;
    }
    else{
        QMessageBox::warning(this,tr("Coordinates"), tr("The coordinates have already been stored"));
    }

}


/*------------
 *
 * ERROR MESSAGES
 *
 * ----------*/

void DMFgui::mapNotCreatedErrorMessage()
{
    QMessageBox::warning(this,tr("Map Creation"), tr("Map is not created yet"));
}


void DMFgui::csvFileNotRead()
{
    QMessageBox::warning(this,tr("csvFile"), tr("This file has not been read yet"));
}

void DMFgui::on_DefaultNumberingBox_clicked(bool checked)
{
    if(checked){
        defaultNumbering=true;
    }
    else
        defaultNumbering= false;
}

void DMFgui::on_filesButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Text files"), "C:/Users/kaleem/Summer_2016/Steve Shih Project", tr("Text Files (*.txt)"));
    ui->textEdit->setPlainText(fileName);
}

void DMFgui::openNewLayoutWindow(){
    nLayout = new newlayout();
    //nLayout->on_defaultNumberingBox_clicked(true);
    nLayout->exec();

    //ui->rowEdit


}