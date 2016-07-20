/********************************************************************************
** Form generated from reading UI file 'savelayout.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVELAYOUT_H
#define UI_SAVELAYOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_savelayout
{
public:

    void setupUi(QDialog *savelayout)
    {
        if (savelayout->objectName().isEmpty())
            savelayout->setObjectName(QStringLiteral("savelayout"));
        savelayout->resize(400, 300);

        retranslateUi(savelayout);

        QMetaObject::connectSlotsByName(savelayout);
    } // setupUi

    void retranslateUi(QDialog *savelayout)
    {
        savelayout->setWindowTitle(QApplication::translate("savelayout", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class savelayout: public Ui_savelayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVELAYOUT_H
