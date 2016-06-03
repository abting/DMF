#include "array_testing.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Array_testing w;
    w.show();

    return a.exec();
}
