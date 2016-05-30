#ifndef ARRAY_TESTING_H
#define ARRAY_TESTING_H

#include <QDialog>
#include <stdio.h>
#include <sstream>

namespace Ui {
class Array_testing;
}

class Array_testing : public QDialog
{
    Q_OBJECT

public:
    explicit Array_testing(QWidget *parent = 0);
    ~Array_testing();

private slots:
    void on_enterButton_clicked();
    void display();

private:
    Ui::Array_testing *ui;

    //std::ostringstream button;
};

#endif // ARRAY_TESTING_H
