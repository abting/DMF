/********************************************************************************
** Form generated from reading UI file 'loadlayout.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADLAYOUT_H
#define UI_LOADLAYOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_loadlayout
{
public:
    QPushButton *openLayoutButton;
    QCheckBox *defaultNumberingBox;
    QPushButton *openContactPadButton;
    QLineEdit *openLayoutEdit;
    QLineEdit *openContactPadEdit;
    QPushButton *enterButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *loadlayout)
    {
        if (loadlayout->objectName().isEmpty())
            loadlayout->setObjectName(QStringLiteral("loadlayout"));
        loadlayout->resize(371, 189);
        openLayoutButton = new QPushButton(loadlayout);
        openLayoutButton->setObjectName(QStringLiteral("openLayoutButton"));
        openLayoutButton->setGeometry(QRect(50, 20, 101, 23));
        defaultNumberingBox = new QCheckBox(loadlayout);
        defaultNumberingBox->setObjectName(QStringLiteral("defaultNumberingBox"));
        defaultNumberingBox->setGeometry(QRect(60, 100, 121, 17));
        openContactPadButton = new QPushButton(loadlayout);
        openContactPadButton->setObjectName(QStringLiteral("openContactPadButton"));
        openContactPadButton->setGeometry(QRect(50, 60, 101, 23));
        openLayoutEdit = new QLineEdit(loadlayout);
        openLayoutEdit->setObjectName(QStringLiteral("openLayoutEdit"));
        openLayoutEdit->setGeometry(QRect(180, 20, 113, 20));
        openContactPadEdit = new QLineEdit(loadlayout);
        openContactPadEdit->setObjectName(QStringLiteral("openContactPadEdit"));
        openContactPadEdit->setGeometry(QRect(180, 60, 113, 20));
        enterButton = new QPushButton(loadlayout);
        enterButton->setObjectName(QStringLiteral("enterButton"));
        enterButton->setGeometry(QRect(180, 150, 75, 23));
        cancelButton = new QPushButton(loadlayout);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(280, 150, 75, 23));

        retranslateUi(loadlayout);

        QMetaObject::connectSlotsByName(loadlayout);
    } // setupUi

    void retranslateUi(QDialog *loadlayout)
    {
        loadlayout->setWindowTitle(QApplication::translate("loadlayout", "Dialog", 0));
        openLayoutButton->setText(QApplication::translate("loadlayout", "Open Layout", 0));
        defaultNumberingBox->setText(QApplication::translate("loadlayout", "Default Numbering", 0));
        openContactPadButton->setText(QApplication::translate("loadlayout", "Open ContactPad", 0));
        enterButton->setText(QApplication::translate("loadlayout", "Enter", 0));
        cancelButton->setText(QApplication::translate("loadlayout", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class loadlayout: public Ui_loadlayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADLAYOUT_H
