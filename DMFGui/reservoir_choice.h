#ifndef RESERVOIR_CHOICE_H
#define RESERVOIR_CHOICE_H

#include <QDialog>
#include "ui_reservoir_choice.h"

namespace Ui {
class Reservoir_choice;
}

class Reservoir_choice : public QDialog
{
    Q_OBJECT

public:
    explicit Reservoir_choice(QWidget *parent = 0);
    ~Reservoir_choice();

private:
    Ui::Reservoir_choice *ui_reservoir;
};

#endif // RESERVOIR_CHOICE_H
