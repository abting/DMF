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

    QString saved;
public slots:
//    void topLeft();
//    void topRight();
//    void bottomLeft();
//    void bottomRight();

    void choice(QStringList);
private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
