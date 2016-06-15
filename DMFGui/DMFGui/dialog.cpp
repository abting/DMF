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

QString Dialog::choice(QString position)
{
    if (position == "topLeft")
    {
        ui->label_2->setText("HOLLA");

        ui->comboBox->addItem("top");
        ui->comboBox->addItem("left");

    }
    else if (position == "topRight")
    {
        ui->comboBox->addItem("top");
        ui->comboBox->addItem("right");
        return on_okButton_clicked();
    }
    else if (position == "bottomRight")
    {
        ui->comboBox->addItem("bottom");
        ui->comboBox->addItem("right");
        return on_okButton_clicked();
    }
    else if (position == "bottomLeft")
    {
        ui->comboBox->addItem("bottom");
        ui->comboBox->addItem("left");
        return on_okButton_clicked();
    }
}

//void Dialog::topLeft()
//{
//    ui->comboBox->addItem("top");
//    ui->comboBox->addItem("left");
//}
//void Dialog::topRight()
//{
//    ui->comboBox->addItem("top");
//    ui->comboBox->addItem("right");
//}
//void Dialog::bottomRight()
//{
//    ui->comboBox->addItem("bottom");
//    ui->comboBox->addItem("right");
//}
//void Dialog::bottomLeft()
//{
//    ui->comboBox->addItem("bottom");
//    ui->comboBox->addItem("left");
//}

//QString Dialog::on_buttonBox_accepted()
//{
//    return ui->comboBox->currentText();
//}

QString Dialog::on_okButton_clicked()
{
    ui->label_2->setText(ui->comboBox->currentText());
    return ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
}
