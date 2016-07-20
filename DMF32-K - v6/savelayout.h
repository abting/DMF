#ifndef SAVELAYOUT_H
#define SAVELAYOUT_H

#include <QDialog>

namespace Ui {
class savelayout;
}

class savelayout : public QDialog
{
    Q_OBJECT

public:
    explicit savelayout(QWidget *parent = 0);
    ~savelayout();

private:
    Ui::savelayout *ui;
};

#endif // SAVELAYOUT_H
