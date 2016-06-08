#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include <QLabel>
#include <QGridLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enterButton_clicked()
{
    QSignalMapper mapper;
    QGridLayout *layout =  new QGridLayout;
    QLabel display;

    const int rows = 4, columns = 3;

    QPushButton **button = new QPushButton*[rows];
            for(int i=0;i<rows;i++){
                button[i]=new QPushButton[columns];
            }

    for (int i = 0; i < rows; ++ i)
    {
       for (int j = 0; j < columns; ++j) {
          QString text = QStringLiteral("(%1,%2)").arg(i).arg(j);
          button[i][j].setText(text);
          layout->addWidget(&button[i][j], i, j);
          mapper.connect(&button[i][j], SIGNAL(clicked()), SLOT(map()));
          mapper.setMapping(&button[i][j], text);
       }
    }
    ui->graphicsView->setLayout(layout);

//    layout->addWidget(&display, rows, 0, 1, columns);

    display.connect(&mapper, SIGNAL(mapped(QString)), SLOT(setText(QString)));
    ui->textEdit->setPlainText(display.text());


}
