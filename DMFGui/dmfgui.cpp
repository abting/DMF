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
int newrow,newcolumn,resnum,corner;
int added = 0;
int elec = 1;

QString to_Send = "";
bool enter_Button_Clicked =false;

bool addRes = false;
int x;
int y;

//Used for Autogen path, color assignments
int* rcoord;
int* ccoord;
int track[4];
int size1;
int firstR,firstC;
boolean autoGen = false;

//for nemesys
double flowRate;
double volume;

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
    ui->unitsComboBox->addItem("mL");
    ui->unitsComboBox->addItem("μL");
    ui->unitsComboBox->addItem("nL");

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

 /*
 *
 *  Kaleem's autogenerate path code.
 *
 *
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
    ui->textEdit->insertPlainText("\ncolI: " +QString::number(colI));
    ui->textEdit->insertPlainText("\nrowI: " +QString::number(rowI));

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
      //save_to_String(retMap[m]);
  }
  //Set final electrode to blue
  dmf_array[rowF][colF].setStyleSheet("background-color:blue; border-style: outset ;border-width: 2px; border-color: grey");
  ui->textEdit->insertPlainText("\ncolF: " +QString::number(colF));
  ui->textEdit->insertPlainText("\nrowF: " +QString::number(rowF));

  //Pointers used to turn colors off
  rcoord = ycoord;
  ccoord = xcoord;
  autoGen = true;
    *
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
    if(autoGen == true){
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
    QApplication::quit();
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
            //naming each of the electrodes
            for (int i=0;i<newrow;i++)
            {
                //count++;
                for (int j=0;j<newcolumn;j++)
                {
                    //add empty and unclickable buttons on the sides
                    if (i==0||i==1||j==0||j==1||i==drow+3||i==drow+2||j==dcolumn+3||j==dcolumn+2)
                    {
                        dmf_array[i][j].setText("");
                        dmf_array[i][j].setEnabled(false);
                        dmf_array[i][j].setStyleSheet("background-color:white;border:none");
                        gridLayout->addWidget(&dmf_array[i][j],i,j);
                    }
                    else
                    {
                        numberingcount++;

                        //inserting values into the array
                        dmf_array[i][j].setText(QString::number(numberingcount));
                        dmf_array[i][j].setStyleSheet( "border-style: outset ;border-width: 2px; border-color: grey");
                        gridLayout->addWidget(&dmf_array[i][j],i,j);
                        mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                        mapper->setMapping(&dmf_array[i][j],QString::number(i)+","+QString::number(j)+","+QString::number(numberingcount));
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

//When an electrode is clicked
void DMFgui::buttonClicked(QString text)
{
    ClearColor();
    QStringList electrodeList;
    electrodeList = text.split(",");
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
               ui->textEdit->insertPlainText("/n in this loop");
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
            //ui->textEdit->insertPlainText("\n called");
            save_to_String(electrodeList.value(2));
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
    dmf_array[y][x].setEnabled(true);
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
   //QString voltage = ui->lineEdit->text(); //gets the text that you've entered

    //float to_Send = voltage.toFloat();
    //connect to the fgen class (generate the commands from there)

    testing = new Testing();

    testing->helloWorld();

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

void DMFgui::on_enterButton_2_clicked(volume,flowRate)
{
    //update the current Values
}

void DMFgui::on_refillButton_clicked(flowRate)
{
    //call the refill function in nemesys
}

void DMFgui::on_emptyButton_clicked(flowRate)
{
    //call the empty function in nemesys
}
