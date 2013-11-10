/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Nov 9 13:27:17 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *captureButton;
    QComboBox *choosePreview;
    QPushButton *faderButton;
    QComboBox *saveBuffer;
    QPushButton *loadButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(176, 288);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        captureButton = new QPushButton(centralWidget);
        captureButton->setObjectName(QString::fromUtf8("captureButton"));
        captureButton->setGeometry(QRect(20, 10, 101, 27));
        choosePreview = new QComboBox(centralWidget);
        choosePreview->setObjectName(QString::fromUtf8("choosePreview"));
        choosePreview->setGeometry(QRect(20, 50, 101, 27));
        faderButton = new QPushButton(centralWidget);
        faderButton->setObjectName(QString::fromUtf8("faderButton"));
        faderButton->setGeometry(QRect(20, 90, 101, 27));
        saveBuffer = new QComboBox(centralWidget);
        saveBuffer->setObjectName(QString::fromUtf8("saveBuffer"));
        saveBuffer->setGeometry(QRect(20, 130, 101, 27));
        loadButton = new QPushButton(centralWidget);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setGeometry(QRect(20, 170, 99, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 176, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Mixer by danman", 0, QApplication::UnicodeUTF8));
        captureButton->setText(QApplication::translate("MainWindow", "Capture...", 0, QApplication::UnicodeUTF8));
        faderButton->setText(QApplication::translate("MainWindow", "Fader...", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("MainWindow", "Load image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
