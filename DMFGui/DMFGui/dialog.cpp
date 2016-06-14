#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::topLeft()
{
    ui->comboBox->addItem("top");
    ui->comboBox->addItem("left");
}
void Dialog::topRight()
{
    ui->comboBox->addItem("top");
    ui->comboBox->addItem("right");
}
void Dialog::bottomRight()
{
    ui->comboBox->addItem("bottom");
    ui->comboBox->addItem("right");
}
void Dialog::bottomLeft()
{
    ui->comboBox->addItem("bottom");
    ui->comboBox->addItem("left");
}

QString Dialog::on_buttonBox_accepted()
{
    return ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
}
