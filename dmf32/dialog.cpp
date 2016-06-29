#include "dialog.h"
#include "ui_dialog.h"

QString toSend = "";

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

void Dialog::choice(QString position)
{
    if (position == "topLeft")
    {
        ui->comboBox->addItem("top");
        ui->comboBox->addItem("left");
    }
    else if (position == "topRight")
    {
        ui->comboBox->addItem("top");
        ui->comboBox->addItem("right");
    }
    else if (position == "bottomRight")
    {
        ui->comboBox->addItem("bottom");
        ui->comboBox->addItem("right");
    }
    else if (position == "bottomLeft")
    {
        ui->comboBox->addItem("bottom");
        ui->comboBox->addItem("left");
    }
}

void Dialog::on_cancelButton_clicked()
{
    QDialog::reject();
    this->close();
}

void Dialog::on_okButton_clicked()
{
    saved = ui->comboBox->currentText();
    QDialog::accept();
    this->close();
}
