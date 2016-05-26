#ifndef DMFGUI_H
#define DMFGUI_H

#include <QDialog>
#include <QSerialPort>
#include <QGraphicsItem>

namespace Ui {
class DMFgui;
}

class DMFgui : public QDialog
{
    Q_OBJECT

public:
    explicit DMFgui(QWidget *parent = 0);
    ~DMFgui();

private slots:

    void on_mixButton_clicked();

    void on_sequenceButton_clicked();

    void on_splitButton_clicked();

    void on_resetButton_clicked();

    void on_exitButton_clicked();

    void on_sendButton_clicked();

    void on_button_1_clicked();

    void on_button_2_clicked();

    void on_button_3_clicked();

    void on_button_4_clicked();

    void on_button_5_clicked();

    void on_button_6_clicked();

    void on_button_7_clicked();

    void on_button_8_clicked();

    void on_button_9_clicked();

    void on_button_10_clicked();

    void updateDMF(QString);

    void save_to_String(QString);

    void on_UndoButton_clicked();

    void set_Scene();

private:
    Ui::DMFgui *ui;

    //initializing arduino here
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
};

#endif // DMFGUI_H
