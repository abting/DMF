#include "loadlayout.h"
#include "ui_loadlayout.h"
#include <QFileDialog>
#include <QMessageBox>


loadlayout::loadlayout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loadlayout)
{
    ui->setupUi(this);
}

loadlayout::~loadlayout()
{
    delete ui;
}

QString loadlayout::on_openLayoutButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Layout Text files"), "C:/Users/umroot/Documents/Working_DMFGUI/DMF32-K -WorkingVersion/LayoutFiles", tr("Text Files (*.txt)"));
    ui->openLayoutEdit->setText(fileName);
    return fileName;
}

QString loadlayout::on_openContactPadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open ContactPad Information Text files"), "C:/Users/umroot/Documents/Working_DMFGUI/DMF32-K -WorkingVersion/ContactPadFiles", tr("Text Files (*.txt)"));
    ui->openContactPadEdit->setText(fileName);
    return fileName;
}

void loadlayout::on_enterButton_clicked()
{


    savedNumbering = ui->defaultNumberingBox->isChecked();
    savedLayoutfilename = ui->openLayoutEdit->text();
    savedContactPadfilename = ui->openContactPadEdit->text();              //Temporary for now to prevent double open

    QDialog::accept();
    this->close();
    QMessageBox::warning(this,"Load Layout Loaded", "You have loaded an existing layout\nYou can now generate the map by pressing\nGenerate Map");

}

void loadlayout::on_cancelButton_clicked()
{
    QDialog::reject();
    this->close();
}

void loadlayout::on_defaultNumberingBox_clicked(bool checked)
{
    if(checked){
        ui->openContactPadButton->setEnabled(false);
    }
    else{
        ui->openContactPadButton->setEnabled(true);
    }
}


