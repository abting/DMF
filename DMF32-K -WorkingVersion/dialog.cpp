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

void Dialog::choice(QStringList positions)
{
    if (positions.contains("top",Qt::CaseSensitive))
    {
        ui->comboBox->addItem("top");
    }
    if (positions.contains("bottom",Qt::CaseSensitive))
    {
        ui->comboBox->addItem("bottom");
    }
    if (positions.contains("left",Qt::CaseSensitive))
    {
        ui->comboBox->addItem("left");
    }
    if (positions.contains("right",Qt::CaseSensitive))
    {
        ui->comboBox->addItem("right");
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
