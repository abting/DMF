#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void topLeft();
    void topRight();
    void bottomLeft();
    void bottomRight();
      QString on_buttonBox_accepted();
private slots:
//    QString on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
