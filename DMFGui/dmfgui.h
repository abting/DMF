#ifndef DMFGUI_H
#define DMFGUI_H

#include <QDialog>
#include <QSerialPort>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "fgen.h"
#include "dialog.h"
#include "testing.h"
#include "nemesys.h"
#include "node.h"

namespace Ui {
class DMFgui;
}

class DMFgui : public QDialog
{
    Q_OBJECT

public:
    explicit DMFgui(QWidget *parent = 0);
    ~DMFgui();

    void mousePressEvent(QMouseEvent *e);
    QString location = "";

public slots:
    QString openNewWindow(int);

private slots:

    void on_mixButton_clicked();
    void on_sequenceButton_clicked();
    void on_splitButton_clicked();
    void on_resetButton_clicked();
    void on_exitButton_clicked();
    void on_sendButton_clicked();
    void on_enterButton_clicked();
    bool add_reservoir();
    void buttonClicked(QString);
    QString autoGeneratePath(const int &,const int &,const int &,const int &);
    void resetMaps();
    QString findAvailableSpace(int, int);
    QString findEmptySpace(int, int);
    void activate(int,int,int);
    void ClearColor();
    int get_Fvalue(int,int,int,int,int,int);

    void updateDMF(QString);

    void save_to_String(QString);

    void on_UndoButton_clicked();

//    void set_Scene();

//    void on_loadButton_clicked();

    void on_Voltage_SendButton_clicked();

    void on_autogen_Button_clicked();

    int* getRecent_Coordinates();

    void setMapping(int, int);

    void on_targetVolume_clicked();

    void on_targetFlow_clicked();

    void on_enterButton_2_clicked();

    void map_reservoir(int,int);
private:
    Ui::DMFgui *ui;
    Dialog *dialog;
    Testing *testing;
    Nemesys *nemesys;

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
