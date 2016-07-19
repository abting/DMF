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
#include "newlayout.h"
#include "loadlayout.h"
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
    //QString openNewWindow(int);

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
    void autoGeneratePath(int,int,int, int, int);
    QString findAvailableSpace(int, int);
    void activate(int,int);
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

    void on_MButton1_clicked();

    void on_MonitorBox1_toggled(bool checked);

    void on_MonitorBox2_toggled(bool checked);

    void on_MButton2_clicked();

    void on_MonitorBox3_toggled(bool checked);

    void on_MButton3_clicked();

    void on_writeButton_clicked();

    void on_ReadButton_clicked();

    int* returnIndex(QPushButton **,int,int,int);

    void mapNotCreatedErrorMessage();

    void on_WritetoTextBox_clicked(bool checked);

    void on_PreviewButton_clicked();

    void on_realTimeBox_clicked(bool checked);

    void on_readCSVButton_clicked();

    void on_fillTextButton_clicked();

    void csvFileNotRead();

    void on_newLayoutButton_clicked();

    void on_loadLayoutButton_clicked(bool checked);

    void on_saveLayoutButton_clicked();

    void on_DeleteBox_clicked(bool checked);

    void openNewLayoutWindow(bool checked);

    QString findEmptySpace(int,int);

    void map_reservoir(int,int);

private:
    Ui::DMFgui *ui;
    Dialog *dialog;
    funcGen *funcgen;
    Nemesys *nemesys;
    newlayout *nLayout;
    loadlayout *lLayout;

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
