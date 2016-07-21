#include "savelayout.h"
#include "ui_savelayout.h"

savelayout::savelayout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::savelayout)
{
    ui->setupUi(this);
}

savelayout::~savelayout()
{
    delete ui;
}
