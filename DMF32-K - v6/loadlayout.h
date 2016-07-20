#ifndef LOADLAYOUT_H
#define LOADLAYOUT_H

#include <QDialog>

namespace Ui {
class loadlayout;
}

class loadlayout : public QDialog
{
    Q_OBJECT

public:
    explicit loadlayout(QWidget *parent = 0);
    ~loadlayout();

    bool savedNumbering;
    QString savedLayoutfilename;
    QString savedContactPadfilename;

private slots:
    QString on_openLayoutButton_clicked();

    QString on_openContactPadButton_clicked();

    void on_enterButton_clicked();

    void on_cancelButton_clicked();

    void on_defaultNumberingBox_clicked(bool checked);


private:
    Ui::loadlayout *ui;
};

#endif // LOADLAYOUT_H
