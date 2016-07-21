/********************************************************************************
** Form generated from reading UI file 'newlayout.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLAYOUT_H
#define UI_NEWLAYOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_newlayout
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *enterButton;
    QPushButton *cancelButton;
    QLineEdit *rowEdit;
    QLineEdit *columnEdit;
    QCheckBox *defaultNumberingBox;
    QPushButton *loadContactPadInfoButton;
    QLineEdit *loadCPadInfoEdit;

    void setupUi(QDialog *newlayout)
    {
        if (newlayout->objectName().isEmpty())
            newlayout->setObjectName(QStringLiteral("newlayout"));
        newlayout->resize(327, 199);
        label = new QLabel(newlayout);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 47, 13));
        label_2 = new QLabel(newlayout);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 50, 47, 13));
        enterButton = new QPushButton(newlayout);
        enterButton->setObjectName(QStringLiteral("enterButton"));
        enterButton->setGeometry(QRect(120, 160, 75, 23));
        cancelButton = new QPushButton(newlayout);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(230, 160, 75, 23));
        rowEdit = new QLineEdit(newlayout);
        rowEdit->setObjectName(QStringLiteral("rowEdit"));
        rowEdit->setGeometry(QRect(100, 20, 61, 16));
        columnEdit = new QLineEdit(newlayout);
        columnEdit->setObjectName(QStringLiteral("columnEdit"));
        columnEdit->setGeometry(QRect(100, 50, 61, 16));
        defaultNumberingBox = new QCheckBox(newlayout);
        defaultNumberingBox->setObjectName(QStringLiteral("defaultNumberingBox"));
        defaultNumberingBox->setGeometry(QRect(30, 80, 111, 17));
        loadContactPadInfoButton = new QPushButton(newlayout);
        loadContactPadInfoButton->setObjectName(QStringLiteral("loadContactPadInfoButton"));
        loadContactPadInfoButton->setGeometry(QRect(30, 110, 121, 23));
        loadCPadInfoEdit = new QLineEdit(newlayout);
        loadCPadInfoEdit->setObjectName(QStringLiteral("loadCPadInfoEdit"));
        loadCPadInfoEdit->setGeometry(QRect(190, 110, 113, 20));

        retranslateUi(newlayout);

        QMetaObject::connectSlotsByName(newlayout);
    } // setupUi

    void retranslateUi(QDialog *newlayout)
    {
        newlayout->setWindowTitle(QApplication::translate("newlayout", "Dialog", 0));
        label->setText(QApplication::translate("newlayout", "Row:", 0));
        label_2->setText(QApplication::translate("newlayout", "Column:", 0));
        enterButton->setText(QApplication::translate("newlayout", "Enter", 0));
        cancelButton->setText(QApplication::translate("newlayout", "Cancel", 0));
        defaultNumberingBox->setText(QApplication::translate("newlayout", "Default Numbering", 0));
        loadContactPadInfoButton->setText(QApplication::translate("newlayout", "Load ContactPad Info", 0));
    } // retranslateUi

};

namespace Ui {
    class newlayout: public Ui_newlayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLAYOUT_H
