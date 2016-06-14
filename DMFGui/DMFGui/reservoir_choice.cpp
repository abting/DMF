#include "reservoir_choice.h"
#include "ui_reservoir_choice.h"

Reservoir_choice::Reservoir_choice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reservoir_choice)
{
    ui->setupUi(this);
}

Reservoir_choice::~Reservoir_choice()
{
    delete ui;
}
