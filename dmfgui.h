#ifndef DMFGUI_H
#define DMFGUI_H

#include <QDialog>
#include <QSerialPort>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "fgen.h"


namespace Ui {
class DMFgui;
}

class DMFgui : public QDialog
{
    Q_OBJECT


public:
    explicit DMFgui(QWidget *parent = 0);
    ~DMFgui();
//    struct coordinates{
//        int x;
//        int y;
//    };



    void mousePressEvent(QMouseEvent *e);
private slots:

    void on_mixButton_clicked();
    void on_sequenceButton_clicked();
    void on_splitButton_clicked();
    void on_resetButton_clicked();
    void on_exitButton_clicked();
    void on_sendButton_clicked();
    void on_enterButton_clicked();
    void add_reservoir(int);
    void buttonClicked(QString);
//    void on_button_1_clicked();
//    void on_button_2_clicked();
//    void on_button_3_clicked();
//    void on_button_4_clicked();
//    void on_button_5_clicked();
//    void on_button_6_clicked();
//    void on_button_7_clicked();
//    void on_button_8_clicked();
//    void on_button_9_clicked();
//    void on_button_10_clicked();
//    void on_button_11_clicked();
//    void on_button_12_clicked();
//    void on_button_13_clicked();
//    void on_button_14_clicked();
//    void on_button_15_clicked();
//    void on_button_16_clicked();
//    void on_button_17_clicked();
//    void on_button_18_clicked();
//    void on_button_19_clicked();
//    void on_button_20_clicked();
//    void on_container_1_clicked();
//    void on_container_2_clicked();
//    void on_container_3_clicked();
//    void on_container_4_clicked();

    void updateDMF(QString);

    void save_to_String(QString);

    void on_UndoButton_clicked();

    void ClearColor();

//    void set_Scene();

//    void on_loadButton_clicked();


    void on_Voltage_SendButton_clicked();

    void on_pushButton_clicked();

    void autoGeneratePath(int,int,int,int,int);



private:
    Ui::DMFgui *ui;

    //initializing arduino here
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

    QGraphicsScene *scene;
    QPixmap image;
    QImage *imageObject;



};

#endif // DMFGUI_H
