/********************************************************************************
** Form generated from reading UI file 'fader.ui'
**
** Created: Sat Nov 9 10:37:24 2013
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
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Fader
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton_3;
    QSlider *horizontalSlider;
    QPushButton *pushButton_2;
    QCheckBox *enableBtn;
    QComboBox *outputSelect;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Fader)
    {
        if (Fader->objectName().isEmpty())
            Fader->setObjectName(QString::fromUtf8("Fader"));
        Fader->resize(752, 432);
        centralwidget = new QWidget(Fader);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(410, 310, 99, 27));
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(260, 270, 181, 29));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setPageStep(10);
        horizontalSlider->setValue(0);
        horizontalSlider->setOrientation(Qt::Horizontal);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(180, 310, 99, 27));
        enableBtn = new QCheckBox(centralwidget);
        enableBtn->setObjectName(QString::fromUtf8("enableBtn"));
        enableBtn->setGeometry(QRect(220, 240, 101, 18));
        outputSelect = new QComboBox(centralwidget);
        outputSelect->setObjectName(QString::fromUtf8("outputSelect"));
        outputSelect->setGeometry(QRect(300, 310, 85, 27));
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
        pushButton_3->setText(QApplication::translate("Fader", "switch B", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Fader", "switch A", 0, QApplication::UnicodeUTF8));
        enableBtn->setText(QApplication::translate("Fader", "enable", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Fader: public Ui_Fader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FADER_H
