#ifndef NEWLAYOUT_H
#define NEWLAYOUT_H

#include <QDialog>

namespace Ui {
class newlayout;
}

class newlayout : public QDialog
{
    Q_OBJECT

public:
    explicit newlayout(QWidget *parent = 0);
    ~newlayout();

     QString savedRow;
     QString savedCol;
     bool savedNumbering;
     QString savedNumberingfilename;


private slots:
    void on_enterButton_clicked();

    void on_cancelButton_clicked();

    QString on_loadContactPadInfoButton_clicked();

    void on_defaultNumberingBox_clicked(bool checked);

private:
    Ui::newlayout *ui;
};

#endif // NEWLAYOUT_H
