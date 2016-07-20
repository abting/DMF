#include "newlayout.h"
#include "ui_newlayout.h"
#include <QFileDialog>
#include <QMessageBox>

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

    savedRow = ui->rowEdit->text();
    savedCol = ui->columnEdit->text();
    if (savedRow.toInt() == 0||savedCol.toInt()==0){                //value entered is not in int, pop up an error message
        QMessageBox::warning(this,tr("Not a number"), tr("Either your row value or column value is not an integer number\nTry again"));
    }
    else{


    savedNumbering = ui->defaultNumberingBox->isChecked();
    savedNumberingfilename = ui->loadCPadInfoEdit->text();              //Temporary for now to prevent double open

    QDialog::accept();
    this->close();
    QMessageBox::warning(this,"Load Layout Loaded", "You have loaded an existing layout\nYou can now generate the map by pressing\nGenerate Map");
    }
}

void newlayout::on_cancelButton_clicked()
{
    QDialog::reject();
    this->close();
}


QString newlayout::on_loadContactPadInfoButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Text files"), "C:/Users/kaleem/Summer_2016/Steve Shih Project/ContactPadFiles", tr("Text Files (*.txt)"));
    ui->loadCPadInfoEdit->setText(fileName);
    return fileName;
}

void newlayout::on_defaultNumberingBox_clicked(bool checked)
{
    if(checked){
        ui->loadContactPadInfoButton->setEnabled(false);
    }
    else{
        ui->loadContactPadInfoButton->setEnabled(true);
    }
}
