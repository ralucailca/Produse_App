/********************************************************************************
** Form generated from reading UI file 'lab14_OOP.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB14_OOP_H
#define UI_LAB14_OOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_lab14_OOPClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *lab14_OOPClass)
    {
        if (lab14_OOPClass->objectName().isEmpty())
            lab14_OOPClass->setObjectName(QString::fromUtf8("lab14_OOPClass"));
        lab14_OOPClass->resize(600, 400);
        menuBar = new QMenuBar(lab14_OOPClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        lab14_OOPClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(lab14_OOPClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        lab14_OOPClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(lab14_OOPClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lab14_OOPClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(lab14_OOPClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        lab14_OOPClass->setStatusBar(statusBar);

        retranslateUi(lab14_OOPClass);

        QMetaObject::connectSlotsByName(lab14_OOPClass);
    } // setupUi

    void retranslateUi(QMainWindow *lab14_OOPClass)
    {
        lab14_OOPClass->setWindowTitle(QApplication::translate("lab14_OOPClass", "lab14_OOP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class lab14_OOPClass: public Ui_lab14_OOPClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB14_OOP_H
