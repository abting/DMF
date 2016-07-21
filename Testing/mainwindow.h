#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "stdlib.h"
#include <queue>
#include <string>
#include "object.h"
using namespace std;

//struct compare
//{
//  bool operator()(const int& l, const int& r)
//  {
//      return l > r;
//  }
//};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void whatever();
public:
    explicit MainWindow(QWidget *parent = 0);
//    bool compare_priority (const Object&,const Object&);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
