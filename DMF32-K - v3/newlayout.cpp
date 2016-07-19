#include "newlayout.h"
#include "ui_newlayout.h"

newlayout::newlayout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newlayout)
{
    ui->setupUi(this);
}

newlayout::~newlayout()
{
    delete ui;
}


void newlayout::on_enterButton_clicked()
{
    QDialog::accept();
    this->close();
}

void newlayout::on_cancelButton_clicked()
{
    QDialog::reject();
    this->close();
}

void newlayout::on_defaultNumberingBox_clicked(bool checked)
{
    if (checked){
        ui->rowEdit->setText("hello");
    }
}
