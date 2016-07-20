/********************************************************************************
** Form generated from reading UI file 'dmfgui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DMFGUI_H
#define UI_DMFGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DMFgui
{
public:
    QTabWidget *tabs;
    QWidget *tab;
    QGroupBox *commandGroup;
    QHBoxLayout *horizontalLayout;
    QRadioButton *mixButton;
    QRadioButton *sequenceButton;
    QRadioButton *splitButton;
    QGroupBox *SerialMonitorGroup;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEdit;
    QPushButton *UndoButton;
    QPushButton *resetButton;
    QPushButton *exitButton;
    QPushButton *sendButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QPushButton *enterButton;
    QPushButton *autogen_Button;
    QPushButton *writeButton;
    QPushButton *ReadButton;
    QCheckBox *WritetoTextBox;
    QLineEdit *PreviewSpeedEdit;
    QLabel *Preview;
    QPushButton *PreviewButton;
    QCheckBox *realTimeBox;
    QPushButton *newLayoutButton;
    QPushButton *loadLayoutButton;
    QPushButton *saveLayoutButton;
    QCheckBox *DeleteBox;
    QWidget *tab_2;
    QPushButton *Voltage_SendButton;
    QLineEdit *lineEdit;
    QLabel *label;
    QWidget *tab_3;
    QComboBox *unitsComboBox;
    QLabel *label_7;
    QPushButton *doseButton;
    QGroupBox *groupBox_2;
    QRadioButton *targetVolume;
    QLabel *label_5;
    QRadioButton *targetFlow;
    QLabel *label_6;
    QLineEdit *targetVolumeEdit;
    QLineEdit *targetFlowRateEdit;
    QPushButton *refillButton;
    QPushButton *emptyButton;
    QGroupBox *groupBox_3;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEdit_2;
    QLineEdit *TargetMonitorFlowRate_1;
    QLineEdit *TargetMonitorSyringeLevel_1;
    QPushButton *OpenButton;
    QPushButton *CloseButton;
    QLabel *label_10;
    QComboBox *funitscomboBox;
    QLabel *label_11;
    QComboBox *dosingUnitscomboBox;
    QPushButton *CalibrateButton;
    QPushButton *StopButton;
    QRadioButton *MButton1;
    QCheckBox *MonitorBox1;
    QLineEdit *targetRefreshRateEdit;
    QLabel *label_12;
    QGroupBox *groupBox_4;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *lineEdit_4;
    QLineEdit *TargetMonitorFlowRate_2;
    QLineEdit *TargetMonitorSyringeLevel_2;
    QGroupBox *groupBox_5;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLineEdit *lineEdit_6;
    QLineEdit *TargetMonitorFlowRate_3;
    QLineEdit *TargetMonitorSyringeLevel_3;
    QRadioButton *MButton2;
    QCheckBox *MonitorBox2;
    QRadioButton *MButton3;
    QCheckBox *MonitorBox3;

    void setupUi(QDialog *DMFgui)
    {
        if (DMFgui->objectName().isEmpty())
            DMFgui->setObjectName(QStringLiteral("DMFgui"));
        DMFgui->resize(872, 409);
        tabs = new QTabWidget(DMFgui);
        tabs->setObjectName(QStringLiteral("tabs"));
        tabs->setGeometry(QRect(0, 0, 871, 401));
        tabs->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        commandGroup = new QGroupBox(tab);
        commandGroup->setObjectName(QStringLiteral("commandGroup"));
        commandGroup->setGeometry(QRect(430, 10, 201, 51));
        horizontalLayout = new QHBoxLayout(commandGroup);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mixButton = new QRadioButton(commandGroup);
        mixButton->setObjectName(QStringLiteral("mixButton"));

        horizontalLayout->addWidget(mixButton);

        sequenceButton = new QRadioButton(commandGroup);
        sequenceButton->setObjectName(QStringLiteral("sequenceButton"));

        horizontalLayout->addWidget(sequenceButton);

        splitButton = new QRadioButton(commandGroup);
        splitButton->setObjectName(QStringLiteral("splitButton"));

        horizontalLayout->addWidget(splitButton);

        SerialMonitorGroup = new QGroupBox(tab);
        SerialMonitorGroup->setObjectName(QStringLiteral("SerialMonitorGroup"));
        SerialMonitorGroup->setGeometry(QRect(420, 80, 441, 221));
        horizontalLayout_2 = new QHBoxLayout(SerialMonitorGroup);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        textEdit = new QTextEdit(SerialMonitorGroup);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        horizontalLayout_2->addWidget(textEdit);

        UndoButton = new QPushButton(tab);
        UndoButton->setObjectName(QStringLiteral("UndoButton"));
        UndoButton->setGeometry(QRect(430, 340, 75, 23));
        resetButton = new QPushButton(tab);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        resetButton->setGeometry(QRect(520, 340, 75, 23));
        exitButton = new QPushButton(tab);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(770, 340, 75, 23));
        sendButton = new QPushButton(tab);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setGeometry(QRect(680, 340, 75, 23));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 80, 401, 221));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graphicsView = new QGraphicsView(groupBox);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        enterButton = new QPushButton(tab);
        enterButton->setObjectName(QStringLiteral("enterButton"));
        enterButton->setGeometry(QRect(150, 50, 91, 31));
        autogen_Button = new QPushButton(tab);
        autogen_Button->setObjectName(QStringLiteral("autogen_Button"));
        autogen_Button->setGeometry(QRect(30, 310, 81, 23));
        writeButton = new QPushButton(tab);
        writeButton->setObjectName(QStringLiteral("writeButton"));
        writeButton->setGeometry(QRect(650, 30, 75, 23));
        ReadButton = new QPushButton(tab);
        ReadButton->setObjectName(QStringLiteral("ReadButton"));
        ReadButton->setGeometry(QRect(750, 30, 75, 23));
        WritetoTextBox = new QCheckBox(tab);
        WritetoTextBox->setObjectName(QStringLiteral("WritetoTextBox"));
        WritetoTextBox->setGeometry(QRect(430, 310, 81, 17));
        PreviewSpeedEdit = new QLineEdit(tab);
        PreviewSpeedEdit->setObjectName(QStringLiteral("PreviewSpeedEdit"));
        PreviewSpeedEdit->setGeometry(QRect(270, 340, 51, 20));
        Preview = new QLabel(tab);
        Preview->setObjectName(QStringLiteral("Preview"));
        Preview->setGeometry(QRect(150, 340, 111, 16));
        PreviewButton = new QPushButton(tab);
        PreviewButton->setObjectName(QStringLiteral("PreviewButton"));
        PreviewButton->setGeometry(QRect(30, 340, 81, 23));
        realTimeBox = new QCheckBox(tab);
        realTimeBox->setObjectName(QStringLiteral("realTimeBox"));
        realTimeBox->setGeometry(QRect(270, 310, 131, 17));
        newLayoutButton = new QPushButton(tab);
        newLayoutButton->setObjectName(QStringLiteral("newLayoutButton"));
        newLayoutButton->setGeometry(QRect(30, 10, 91, 31));
        loadLayoutButton = new QPushButton(tab);
        loadLayoutButton->setObjectName(QStringLiteral("loadLayoutButton"));
        loadLayoutButton->setGeometry(QRect(150, 10, 91, 31));
        saveLayoutButton = new QPushButton(tab);
        saveLayoutButton->setObjectName(QStringLiteral("saveLayoutButton"));
        saveLayoutButton->setGeometry(QRect(280, 10, 81, 31));
        DeleteBox = new QCheckBox(tab);
        DeleteBox->setObjectName(QStringLiteral("DeleteBox"));
        DeleteBox->setGeometry(QRect(140, 310, 101, 17));
        tabs->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        Voltage_SendButton = new QPushButton(tab_2);
        Voltage_SendButton->setObjectName(QStringLiteral("Voltage_SendButton"));
        Voltage_SendButton->setGeometry(QRect(230, 30, 75, 23));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 30, 121, 21));
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 121, 21));
        tabs->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        unitsComboBox = new QComboBox(tab_3);
        unitsComboBox->setObjectName(QStringLiteral("unitsComboBox"));
        unitsComboBox->setGeometry(QRect(120, 50, 71, 20));
        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(30, 50, 71, 16));
        doseButton = new QPushButton(tab_3);
        doseButton->setObjectName(QStringLiteral("doseButton"));
        doseButton->setGeometry(QRect(10, 230, 51, 23));
        groupBox_2 = new QGroupBox(tab_3);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 120, 171, 91));
        targetVolume = new QRadioButton(groupBox_2);
        targetVolume->setObjectName(QStringLiteral("targetVolume"));
        targetVolume->setGeometry(QRect(10, 20, 21, 17));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(100, 20, 47, 13));
        targetFlow = new QRadioButton(groupBox_2);
        targetFlow->setObjectName(QStringLiteral("targetFlow"));
        targetFlow->setGeometry(QRect(10, 50, 21, 17));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(100, 50, 47, 13));
        targetVolumeEdit = new QLineEdit(groupBox_2);
        targetVolumeEdit->setObjectName(QStringLiteral("targetVolumeEdit"));
        targetVolumeEdit->setGeometry(QRect(30, 20, 61, 20));
        targetFlowRateEdit = new QLineEdit(groupBox_2);
        targetFlowRateEdit->setObjectName(QStringLiteral("targetFlowRateEdit"));
        targetFlowRateEdit->setGeometry(QRect(30, 50, 61, 20));
        refillButton = new QPushButton(tab_3);
        refillButton->setObjectName(QStringLiteral("refillButton"));
        refillButton->setGeometry(QRect(80, 230, 61, 23));
        emptyButton = new QPushButton(tab_3);
        emptyButton->setObjectName(QStringLiteral("emptyButton"));
        emptyButton->setGeometry(QRect(160, 230, 61, 23));
        groupBox_3 = new QGroupBox(tab_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(270, 60, 151, 121));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 20, 47, 13));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 50, 51, 16));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 80, 71, 16));
        lineEdit_2 = new QLineEdit(groupBox_3);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(70, 20, 71, 20));
        TargetMonitorFlowRate_1 = new QLineEdit(groupBox_3);
        TargetMonitorFlowRate_1->setObjectName(QStringLiteral("TargetMonitorFlowRate_1"));
        TargetMonitorFlowRate_1->setGeometry(QRect(80, 50, 61, 20));
        TargetMonitorSyringeLevel_1 = new QLineEdit(groupBox_3);
        TargetMonitorSyringeLevel_1->setObjectName(QStringLiteral("TargetMonitorSyringeLevel_1"));
        TargetMonitorSyringeLevel_1->setGeometry(QRect(90, 80, 51, 20));
        OpenButton = new QPushButton(tab_3);
        OpenButton->setObjectName(QStringLiteral("OpenButton"));
        OpenButton->setGeometry(QRect(290, 2, 101, 41));
        CloseButton = new QPushButton(tab_3);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(410, 2, 101, 41));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(30, 80, 81, 16));
        funitscomboBox = new QComboBox(tab_3);
        funitscomboBox->setObjectName(QStringLiteral("funitscomboBox"));
        funitscomboBox->setGeometry(QRect(120, 80, 69, 21));
        label_11 = new QLabel(tab_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(30, 20, 71, 16));
        dosingUnitscomboBox = new QComboBox(tab_3);
        dosingUnitscomboBox->setObjectName(QStringLiteral("dosingUnitscomboBox"));
        dosingUnitscomboBox->setGeometry(QRect(120, 20, 71, 20));
        CalibrateButton = new QPushButton(tab_3);
        CalibrateButton->setObjectName(QStringLiteral("CalibrateButton"));
        CalibrateButton->setGeometry(QRect(130, 260, 71, 31));
        StopButton = new QPushButton(tab_3);
        StopButton->setObjectName(QStringLiteral("StopButton"));
        StopButton->setGeometry(QRect(30, 260, 71, 31));
        MButton1 = new QRadioButton(tab_3);
        MButton1->setObjectName(QStringLiteral("MButton1"));
        MButton1->setEnabled(false);
        MButton1->setGeometry(QRect(290, 210, 71, 31));
        MonitorBox1 = new QCheckBox(tab_3);
        MonitorBox1->setObjectName(QStringLiteral("MonitorBox1"));
        MonitorBox1->setGeometry(QRect(290, 190, 91, 17));
        targetRefreshRateEdit = new QLineEdit(tab_3);
        targetRefreshRateEdit->setObjectName(QStringLiteral("targetRefreshRateEdit"));
        targetRefreshRateEdit->setGeometry(QRect(430, 270, 81, 20));
        label_12 = new QLabel(tab_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(320, 270, 101, 16));
        groupBox_4 = new QGroupBox(tab_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(430, 60, 151, 121));
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(20, 20, 47, 13));
        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(20, 50, 51, 16));
        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(20, 80, 71, 16));
        lineEdit_4 = new QLineEdit(groupBox_4);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(70, 20, 71, 20));
        TargetMonitorFlowRate_2 = new QLineEdit(groupBox_4);
        TargetMonitorFlowRate_2->setObjectName(QStringLiteral("TargetMonitorFlowRate_2"));
        TargetMonitorFlowRate_2->setGeometry(QRect(80, 50, 61, 20));
        TargetMonitorSyringeLevel_2 = new QLineEdit(groupBox_4);
        TargetMonitorSyringeLevel_2->setObjectName(QStringLiteral("TargetMonitorSyringeLevel_2"));
        TargetMonitorSyringeLevel_2->setGeometry(QRect(90, 80, 51, 20));
        groupBox_5 = new QGroupBox(tab_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(590, 60, 151, 121));
        label_22 = new QLabel(groupBox_5);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(20, 20, 47, 13));
        label_23 = new QLabel(groupBox_5);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(20, 50, 51, 16));
        label_24 = new QLabel(groupBox_5);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(20, 80, 71, 16));
        lineEdit_6 = new QLineEdit(groupBox_5);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(70, 20, 71, 20));
        TargetMonitorFlowRate_3 = new QLineEdit(groupBox_5);
        TargetMonitorFlowRate_3->setObjectName(QStringLiteral("TargetMonitorFlowRate_3"));
        TargetMonitorFlowRate_3->setGeometry(QRect(80, 50, 61, 20));
        TargetMonitorSyringeLevel_3 = new QLineEdit(groupBox_5);
        TargetMonitorSyringeLevel_3->setObjectName(QStringLiteral("TargetMonitorSyringeLevel_3"));
        TargetMonitorSyringeLevel_3->setGeometry(QRect(90, 80, 51, 20));
        MButton2 = new QRadioButton(tab_3);
        MButton2->setObjectName(QStringLiteral("MButton2"));
        MButton2->setEnabled(false);
        MButton2->setGeometry(QRect(470, 210, 71, 31));
        MonitorBox2 = new QCheckBox(tab_3);
        MonitorBox2->setObjectName(QStringLiteral("MonitorBox2"));
        MonitorBox2->setGeometry(QRect(470, 190, 91, 17));
        MButton3 = new QRadioButton(tab_3);
        MButton3->setObjectName(QStringLiteral("MButton3"));
        MButton3->setEnabled(false);
        MButton3->setGeometry(QRect(640, 210, 71, 31));
        MonitorBox3 = new QCheckBox(tab_3);
        MonitorBox3->setObjectName(QStringLiteral("MonitorBox3"));
        MonitorBox3->setGeometry(QRect(640, 190, 91, 17));
        tabs->addTab(tab_3, QString());
        MButton1->raise();
        unitsComboBox->raise();
        label_7->raise();
        doseButton->raise();
        groupBox_2->raise();
        refillButton->raise();
        emptyButton->raise();
        groupBox_3->raise();
        OpenButton->raise();
        CloseButton->raise();
        label_10->raise();
        funitscomboBox->raise();
        label_11->raise();
        dosingUnitscomboBox->raise();
        CalibrateButton->raise();
        StopButton->raise();
        MonitorBox1->raise();
        targetRefreshRateEdit->raise();
        label_12->raise();
        groupBox_4->raise();
        groupBox_5->raise();
        MButton2->raise();
        MonitorBox2->raise();
        MButton3->raise();
        MonitorBox3->raise();
        groupBox_2->raise();
        funitscomboBox->raise();
        refillButton->raise();
        StopButton->raise();
        emptyButton->raise();
        unitsComboBox->raise();
        doseButton->raise();

        retranslateUi(DMFgui);

        tabs->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(DMFgui);
    } // setupUi

    void retranslateUi(QDialog *DMFgui)
    {
        DMFgui->setWindowTitle(QApplication::translate("DMFgui", "DMFgui", 0));
        commandGroup->setTitle(QApplication::translate("DMFgui", "Command", 0));
        mixButton->setText(QApplication::translate("DMFgui", "Mix", 0));
        sequenceButton->setText(QApplication::translate("DMFgui", "Sequence", 0));
        splitButton->setText(QApplication::translate("DMFgui", "Split", 0));
        SerialMonitorGroup->setTitle(QApplication::translate("DMFgui", "Serial Monitor", 0));
        UndoButton->setText(QApplication::translate("DMFgui", "Undo", 0));
        resetButton->setText(QApplication::translate("DMFgui", "Reset", 0));
        exitButton->setText(QApplication::translate("DMFgui", "Exit", 0));
        sendButton->setText(QApplication::translate("DMFgui", "Send", 0));
        groupBox->setTitle(QApplication::translate("DMFgui", "DMF Device", 0));
        enterButton->setText(QApplication::translate("DMFgui", "Generate Map", 0));
        autogen_Button->setText(QApplication::translate("DMFgui", "AutoGen", 0));
        writeButton->setText(QApplication::translate("DMFgui", "Write", 0));
        ReadButton->setText(QApplication::translate("DMFgui", "Read", 0));
        WritetoTextBox->setText(QApplication::translate("DMFgui", "WritetoText", 0));
        Preview->setText(QApplication::translate("DMFgui", "Preview Speed (secs):", 0));
        PreviewButton->setText(QApplication::translate("DMFgui", "Preview", 0));
        realTimeBox->setText(QApplication::translate("DMFgui", "Real-time sequencing", 0));
        newLayoutButton->setText(QApplication::translate("DMFgui", "New Layout", 0));
        loadLayoutButton->setText(QApplication::translate("DMFgui", "Load Layout", 0));
        saveLayoutButton->setText(QApplication::translate("DMFgui", "Save Layout", 0));
        DeleteBox->setText(QApplication::translate("DMFgui", "Delete Buttons", 0));
        tabs->setTabText(tabs->indexOf(tab), QApplication::translate("DMFgui", "DMF", 0));
        Voltage_SendButton->setText(QApplication::translate("DMFgui", "Send", 0));
        lineEdit->setText(QString());
        label->setText(QApplication::translate("DMFgui", "Set Voltage:", 0));
        tabs->setTabText(tabs->indexOf(tab_2), QApplication::translate("DMFgui", "Function Generator", 0));
        label_7->setText(QApplication::translate("DMFgui", "Volume Units:", 0));
        doseButton->setText(QApplication::translate("DMFgui", "Dose", 0));
        groupBox_2->setTitle(QApplication::translate("DMFgui", "Target Values", 0));
        targetVolume->setText(QString());
        label_5->setText(QApplication::translate("DMFgui", "Volume", 0));
        targetFlow->setText(QString());
        label_6->setText(QApplication::translate("DMFgui", "Flow rate", 0));
        refillButton->setText(QApplication::translate("DMFgui", "Refill", 0));
        emptyButton->setText(QApplication::translate("DMFgui", "Empty", 0));
        groupBox_3->setTitle(QApplication::translate("DMFgui", "Current Values Dosing Unit 1", 0));
        label_4->setText(QApplication::translate("DMFgui", "Volume:", 0));
        label_8->setText(QApplication::translate("DMFgui", "Flow rate:", 0));
        label_9->setText(QApplication::translate("DMFgui", "Syringe Level:", 0));
        OpenButton->setText(QApplication::translate("DMFgui", "Open NemeSYS", 0));
        CloseButton->setText(QApplication::translate("DMFgui", "Close NemeSYS", 0));
        label_10->setText(QApplication::translate("DMFgui", "Flow rate Units:", 0));
        label_11->setText(QApplication::translate("DMFgui", "Dosing Units:", 0));
        CalibrateButton->setText(QApplication::translate("DMFgui", "Calibrate", 0));
        StopButton->setText(QApplication::translate("DMFgui", "Stop", 0));
        MButton1->setText(QApplication::translate("DMFgui", "MButton", 0));
        MonitorBox1->setText(QApplication::translate("DMFgui", "Monitor Unit 1", 0));
        label_12->setText(QApplication::translate("DMFgui", "Refresh Rate (ms):", 0));
        groupBox_4->setTitle(QApplication::translate("DMFgui", "Current Values Dosing Unit 2", 0));
        label_16->setText(QApplication::translate("DMFgui", "Volume:", 0));
        label_17->setText(QApplication::translate("DMFgui", "Flow rate:", 0));
        label_18->setText(QApplication::translate("DMFgui", "Syringe Level:", 0));
        groupBox_5->setTitle(QApplication::translate("DMFgui", "Current Values Dosing Unit 3", 0));
        label_22->setText(QApplication::translate("DMFgui", "Volume:", 0));
        label_23->setText(QApplication::translate("DMFgui", "Flow rate:", 0));
        label_24->setText(QApplication::translate("DMFgui", "Syringe Level:", 0));
        MButton2->setText(QApplication::translate("DMFgui", "MButton", 0));
        MonitorBox2->setText(QApplication::translate("DMFgui", "Monitor Unit 2", 0));
        MButton3->setText(QApplication::translate("DMFgui", "MButton", 0));
        MonitorBox3->setText(QApplication::translate("DMFgui", "Monitor Unit 3", 0));
        tabs->setTabText(tabs->indexOf(tab_3), QApplication::translate("DMFgui", "Nemesys", 0));
    } // retranslateUi

};

namespace Ui {
    class DMFgui: public Ui_DMFgui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DMFGUI_H
