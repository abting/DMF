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

QString to_Send = "";
QString to_Display = "";
int nextLineCount = 0;
QString lineTrack = "";
bool finished_Clicking = false;

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
        int newrow = drow+4;
        int newcolumn = dcolumn+4;

        QPushButton **dmf_array = new QPushButton*[newrow];
                for(int i=0;i<newrow;i++){
                    dmf_array[i]=new QPushButton[newcolumn];
                }
        QLabel *empty = new QLabel(this);

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

                    mapper->connect(&dmf_array[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
                    mapper->setMapping(&dmf_array[i][j],QString::number(numberingcount));
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
//    ui->label->setText(x);
//    ui->textEdit->setPlainText(ui->label->text());
    save_to_String(text);
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
