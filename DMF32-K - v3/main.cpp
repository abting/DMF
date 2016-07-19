#include "dmfgui.h"
#include <QApplication>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DMFgui w;
    w.setWindowTitle("DMF Controller");
    w.show();

    return a.exec();
}
