/********************************************************************************
** Form generated from reading UI file 'fader.ui'
**
** Created: Sun Nov 10 10:36:30 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FADER_H
#define UI_FADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Fader
{
public:
    QWidget *centralwidget;
    QSlider *horizontalSlider;
    QCheckBox *enableBtn;
    QComboBox *outputSelect;
    QFrame *redFrame;
    QRadioButton *red1;
    QRadioButton *red2;
    QRadioButton *red3;
    QRadioButton *red4;
    QRadioButton *red5;
    QRadioButton *red6;
    QRadioButton *red7;
    QRadioButton *red0;
    QFrame *yelFrame;
    QRadioButton *yel0;
    QRadioButton *yel1;
    QRadioButton *yel2;
    QRadioButton *yel3;
    QRadioButton *yel4;
    QRadioButton *yel5;
    QRadioButton *yel6;
    QRadioButton *yel7;
    QSpinBox *fadeLength;
    QPushButton *fadeButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QButtonGroup *redButtons;
    QButtonGroup *yellowButtons;

    void setupUi(QMainWindow *Fader)
    {
        if (Fader->objectName().isEmpty())
            Fader->setObjectName(QString::fromUtf8("Fader"));
        Fader->resize(752, 432);
        centralwidget = new QWidget(Fader);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(220, 300, 261, 29));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setPageStep(10);
        horizontalSlider->setValue(0);
        horizontalSlider->setOrientation(Qt::Horizontal);
        enableBtn = new QCheckBox(centralwidget);
        enableBtn->setObjectName(QString::fromUtf8("enableBtn"));
        enableBtn->setGeometry(QRect(30, 30, 101, 18));
        outputSelect = new QComboBox(centralwidget);
        outputSelect->setObjectName(QString::fromUtf8("outputSelect"));
        outputSelect->setGeometry(QRect(140, 20, 85, 27));
        redFrame = new QFrame(centralwidget);
        redFrame->setObjectName(QString::fromUtf8("redFrame"));
        redFrame->setGeometry(QRect(30, 80, 651, 81));
        redFrame->setFrameShape(QFrame::StyledPanel);
        redFrame->setFrameShadow(QFrame::Raised);
        red1 = new QRadioButton(redFrame);
        redButtons = new QButtonGroup(Fader);
        redButtons->setObjectName(QString::fromUtf8("redButtons"));
        redButtons->addButton(red1);
        red1->setObjectName(QString::fromUtf8("red1"));
        red1->setGeometry(QRect(110, 10, 51, 61));
        red1->setChecked(false);
        red2 = new QRadioButton(redFrame);
        redButtons->addButton(red2);
        red2->setObjectName(QString::fromUtf8("red2"));
        red2->setGeometry(QRect(190, 10, 51, 61));
        red2->setChecked(false);
        red3 = new QRadioButton(redFrame);
        redButtons->addButton(red3);
        red3->setObjectName(QString::fromUtf8("red3"));
        red3->setGeometry(QRect(270, 10, 51, 61));
        red3->setChecked(false);
        red4 = new QRadioButton(redFrame);
        redButtons->addButton(red4);
        red4->setObjectName(QString::fromUtf8("red4"));
        red4->setGeometry(QRect(350, 10, 51, 61));
        red4->setChecked(false);
        red5 = new QRadioButton(redFrame);
        redButtons->addButton(red5);
        red5->setObjectName(QString::fromUtf8("red5"));
        red5->setGeometry(QRect(430, 10, 51, 61));
        red5->setChecked(false);
        red6 = new QRadioButton(redFrame);
        redButtons->addButton(red6);
        red6->setObjectName(QString::fromUtf8("red6"));
        red6->setGeometry(QRect(500, 10, 51, 61));
        red6->setChecked(false);
        red7 = new QRadioButton(redFrame);
        redButtons->addButton(red7);
        red7->setObjectName(QString::fromUtf8("red7"));
        red7->setGeometry(QRect(570, 10, 51, 61));
        red7->setChecked(false);
        red0 = new QRadioButton(redFrame);
        redButtons->addButton(red0);
        red0->setObjectName(QString::fromUtf8("red0"));
        red0->setGeometry(QRect(30, 10, 51, 61));
        red0->setChecked(true);
        yelFrame = new QFrame(centralwidget);
        yelFrame->setObjectName(QString::fromUtf8("yelFrame"));
        yelFrame->setGeometry(QRect(30, 180, 651, 80));
        yelFrame->setFrameShape(QFrame::StyledPanel);
        yelFrame->setFrameShadow(QFrame::Raised);
        yel0 = new QRadioButton(yelFrame);
        yellowButtons = new QButtonGroup(Fader);
        yellowButtons->setObjectName(QString::fromUtf8("yellowButtons"));
        yellowButtons->addButton(yel0);
        yel0->setObjectName(QString::fromUtf8("yel0"));
        yel0->setGeometry(QRect(30, 10, 51, 61));
        yel0->setChecked(true);
        yel1 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel1);
        yel1->setObjectName(QString::fromUtf8("yel1"));
        yel1->setGeometry(QRect(110, 10, 51, 61));
        yel1->setChecked(false);
        yel2 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel2);
        yel2->setObjectName(QString::fromUtf8("yel2"));
        yel2->setGeometry(QRect(190, 10, 51, 61));
        yel2->setChecked(false);
        yel3 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel3);
        yel3->setObjectName(QString::fromUtf8("yel3"));
        yel3->setGeometry(QRect(270, 10, 51, 61));
        yel3->setChecked(false);
        yel4 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel4);
        yel4->setObjectName(QString::fromUtf8("yel4"));
        yel4->setGeometry(QRect(350, 10, 51, 61));
        yel4->setChecked(false);
        yel5 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel5);
        yel5->setObjectName(QString::fromUtf8("yel5"));
        yel5->setGeometry(QRect(430, 10, 51, 61));
        yel5->setChecked(false);
        yel6 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel6);
        yel6->setObjectName(QString::fromUtf8("yel6"));
        yel6->setGeometry(QRect(500, 10, 51, 61));
        yel6->setChecked(false);
        yel7 = new QRadioButton(yelFrame);
        yellowButtons->addButton(yel7);
        yel7->setObjectName(QString::fromUtf8("yel7"));
        yel7->setGeometry(QRect(570, 10, 51, 61));
        yel7->setChecked(false);
        fadeLength = new QSpinBox(centralwidget);
        fadeLength->setObjectName(QString::fromUtf8("fadeLength"));
        fadeLength->setGeometry(QRect(240, 340, 71, 27));
        fadeLength->setMinimum(1);
        fadeLength->setMaximum(10000);
        fadeLength->setSingleStep(100);
        fadeLength->setValue(400);
        fadeButton = new QPushButton(centralwidget);
        fadeButton->setObjectName(QString::fromUtf8("fadeButton"));
        fadeButton->setGeometry(QRect(370, 340, 99, 27));
        Fader->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Fader);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 752, 25));
        Fader->setMenuBar(menubar);
        statusbar = new QStatusBar(Fader);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Fader->setStatusBar(statusbar);

        retranslateUi(Fader);

        QMetaObject::connectSlotsByName(Fader);
    } // setupUi

    void retranslateUi(QMainWindow *Fader)
    {
        Fader->setWindowTitle(QApplication::translate("Fader", "MainWindow", 0, QApplication::UnicodeUTF8));
        enableBtn->setText(QApplication::translate("Fader", "enable", 0, QApplication::UnicodeUTF8));
        red1->setText(QApplication::translate("Fader", "1", 0, QApplication::UnicodeUTF8));
        red2->setText(QApplication::translate("Fader", "2", 0, QApplication::UnicodeUTF8));
        red3->setText(QApplication::translate("Fader", "3", 0, QApplication::UnicodeUTF8));
        red4->setText(QApplication::translate("Fader", "4", 0, QApplication::UnicodeUTF8));
        red5->setText(QApplication::translate("Fader", "5", 0, QApplication::UnicodeUTF8));
        red6->setText(QApplication::translate("Fader", "6", 0, QApplication::UnicodeUTF8));
        red7->setText(QApplication::translate("Fader", "7", 0, QApplication::UnicodeUTF8));
        red0->setText(QApplication::translate("Fader", "0", 0, QApplication::UnicodeUTF8));
        yel0->setText(QApplication::translate("Fader", "0", 0, QApplication::UnicodeUTF8));
        yel1->setText(QApplication::translate("Fader", "1", 0, QApplication::UnicodeUTF8));
        yel2->setText(QApplication::translate("Fader", "2", 0, QApplication::UnicodeUTF8));
        yel3->setText(QApplication::translate("Fader", "3", 0, QApplication::UnicodeUTF8));
        yel4->setText(QApplication::translate("Fader", "4", 0, QApplication::UnicodeUTF8));
        yel5->setText(QApplication::translate("Fader", "5", 0, QApplication::UnicodeUTF8));
        yel6->setText(QApplication::translate("Fader", "6", 0, QApplication::UnicodeUTF8));
        yel7->setText(QApplication::translate("Fader", "7", 0, QApplication::UnicodeUTF8));
        fadeButton->setText(QApplication::translate("Fader", "Autofade", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Fader: public Ui_Fader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FADER_H
