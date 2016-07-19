#ifndef DMFGUI_H
#define DMFGUI_H

#include <QDialog>
#include <QSerialPort>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "funcgen.h"
#include "dialog.h"
#include "visa.h"
#include "visatype.h"
#include "nemesys.h"
#include "node.h"
using namespace std;
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
    void on_Voltage_SendButton_clicked();

    void on_OpenButton_clicked();

    void on_CloseButton_clicked();

private slots:

    void on_mixButton_clicked();
    void on_sequenceButton_clicked();
    void on_splitButton_clicked();
    void on_resetButton_clicked();
    void on_exitButton_clicked();
    void on_sendButton_clicked();
    void on_enterButton_clicked();
    bool add_reservoir(int,int,int);
    void buttonClicked(QString);
    QString autoGeneratePath(const int &,const int &,const int &,const int &);
    void buildArray(int, int);
    QString findEmptySpace(int, int);
    void activate(int,int,int);
    void ClearColor();

    void updateDMF(QString);

    void save_to_String(QString);

    void on_UndoButton_clicked();

//    void set_Scene();

//    void on_loadButton_clicked();

    void on_autogen_Button_clicked();

    int* getRecent_Coordinates();

    void setMapping(int, int);

    void on_doseButton_clicked();

    void on_targetVolume_clicked();

    void on_targetFlow_clicked();

    void nemesysNotOpenedErrorMessage();

    void nemesysAlreadyOpenedMessage();

    void on_emptyButton_clicked();

    void nemesysDosingMessage();

    void nemesysCalibrateMessage();

    void on_refillButton_clicked();

    void on_CalibrateButton_clicked();

    void closeEvent(QCloseEvent *);

    void on_StopButton_clicked();

    void on_writeButton_clicked();

    void on_readButton_clicked();

    void on_previewButton_clicked();

    int* returnIndex(QPushButton** ,int , int , int );

    void mapNotCreatedErrorMessage();

    void map_reservoir(int , int);

    void on_writeToMonitorBox_clicked(bool checked);

    void on_MonitorBox1_clicked(bool checked);

    void on_MonitorBox2_clicked(bool checked);

    void on_MonitorBox3_clicked(bool checked);

    void on_MButton1_clicked();

    void on_MButton2_clicked();

    void on_MButton3_clicked();

    void on_readPCBinfoButton_clicked();

    void on_realTimeSequencingBox_clicked(bool checked);

private:
    Ui::DMFgui *ui;
    Dialog *dialog;
    funcGen *funcgen;
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
