/********************************************************************************
** Form generated from reading UI file 'capture.ui'
**
** Created: Fri Oct 25 23:39:48 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAPTURE_H
#define UI_CAPTURE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Capture
{
public:
    QCheckBox *enableInput;
    QFrame *ctrlFrame;
    QComboBox *inputName;
    QSpinBox *outputNumber;
    QPlainTextEdit *frameCount;

    void setupUi(QWidget *Capture)
    {
        if (Capture->objectName().isEmpty())
            Capture->setObjectName(QString::fromUtf8("Capture"));
        Capture->resize(194, 203);
        Capture->setAutoFillBackground(false);
        enableInput = new QCheckBox(Capture);
        enableInput->setObjectName(QString::fromUtf8("enableInput"));
        enableInput->setGeometry(QRect(20, 10, 98, 22));
        ctrlFrame = new QFrame(Capture);
        ctrlFrame->setObjectName(QString::fromUtf8("ctrlFrame"));
        ctrlFrame->setGeometry(QRect(20, 80, 151, 101));
        ctrlFrame->setFrameShape(QFrame::StyledPanel);
        ctrlFrame->setFrameShadow(QFrame::Raised);
        inputName = new QComboBox(ctrlFrame);
        inputName->setObjectName(QString::fromUtf8("inputName"));
        inputName->setGeometry(QRect(10, 20, 121, 27));
        outputNumber = new QSpinBox(ctrlFrame);
        outputNumber->setObjectName(QString::fromUtf8("outputNumber"));
        outputNumber->setGeometry(QRect(10, 60, 121, 27));
        frameCount = new QPlainTextEdit(Capture);
        frameCount->setObjectName(QString::fromUtf8("frameCount"));
        frameCount->setEnabled(false);
        frameCount->setGeometry(QRect(20, 40, 121, 31));
        frameCount->setReadOnly(true);

        retranslateUi(Capture);

        QMetaObject::connectSlotsByName(Capture);
    } // setupUi

    void retranslateUi(QWidget *Capture)
    {
        Capture->setWindowTitle(QApplication::translate("Capture", "Capture", 0, QApplication::UnicodeUTF8));
        enableInput->setText(QApplication::translate("Capture", "capture", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Capture: public Ui_Capture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAPTURE_H
