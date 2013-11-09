/********************************************************************************
** Form generated from reading UI file 'preview.ui'
**
** Created: Sat Nov 9 11:05:13 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEW_H
#define UI_PREVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Preview
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;

    void setupUi(QMainWindow *Preview)
    {
        if (Preview->objectName().isEmpty())
            Preview->setObjectName(QString::fromUtf8("Preview"));
        Preview->resize(343, 290);
        centralwidget = new QWidget(Preview);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Preview->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Preview);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 343, 25));
        Preview->setMenuBar(menubar);

        retranslateUi(Preview);

        QMetaObject::connectSlotsByName(Preview);
    } // setupUi

    void retranslateUi(QMainWindow *Preview)
    {
        Preview->setWindowTitle(QApplication::translate("Preview", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Preview: public Ui_Preview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEW_H
