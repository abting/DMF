#include "dmfgui.h"
#include "ui_dmfgui.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

QString to_Send = "";
QString to_Display = "";
int nextLineCount = 0;
QString lineTrack = "";

DMFgui::DMFgui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DMFgui)
{
    ui->setupUi(this);
    set_Scene();

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

void DMFgui::on_button_1_clicked()
{
     save_to_String("1");
}

void DMFgui::on_button_2_clicked()
{
    save_to_String("2");
}

void DMFgui::on_button_3_clicked()
{
    save_to_String("3");
}

void DMFgui::on_button_4_clicked()
{
    save_to_String("4");
}

void DMFgui::on_button_5_clicked()
{
    save_to_String("5");
}

void DMFgui::on_button_6_clicked()
{
    save_to_String("6");
}

void DMFgui::on_button_7_clicked()
{
    save_to_String("7");
}

void DMFgui::on_button_8_clicked()
{
    save_to_String("8");
}

void DMFgui::on_button_9_clicked()
{
    save_to_String("9");
}

void DMFgui::on_button_10_clicked()
{
    save_to_String("10");
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

void DMFgui::set_Scene()
{
//    QImage DMF_image(":/DMF_Scene.png");

//    //testing the graphicsView item
//    scene = new QGraphicsScene(this); //setting object

//    QGraphicsPixmapItem item(QPixmap("c:\\DMF_Scene.png"));
//    scene->addItem(&item);

//    ui->graphicsView->setScene(scene);
//    scene->addPixmap(QPixmap::fromImage(DMF_image));
//    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

//    QBrush redBrush(Qt::red);
//    QBrush blueBrush(Qt::blue);
//    QPen blackpen(Qt::black);
//    blackpen.setWidth(6);
//    ellipse = scene->addEllipse(10,10,100,100,blackpen,redBrush);

      QGraphicsPixmapItem item(QPixmap::fromImage(DMF_image));
      scene->addItem(&item);

}
