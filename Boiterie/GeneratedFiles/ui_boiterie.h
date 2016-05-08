/********************************************************************************
** Form generated from reading UI file 'boiterie.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOITERIE_H
#define UI_BOITERIE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoiterieClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BoiterieClass)
    {
        if (BoiterieClass->objectName().isEmpty())
            BoiterieClass->setObjectName(QStringLiteral("BoiterieClass"));
        BoiterieClass->resize(600, 400);
        menuBar = new QMenuBar(BoiterieClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        BoiterieClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(BoiterieClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        BoiterieClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(BoiterieClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BoiterieClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(BoiterieClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BoiterieClass->setStatusBar(statusBar);

        retranslateUi(BoiterieClass);

        QMetaObject::connectSlotsByName(BoiterieClass);
    } // setupUi

    void retranslateUi(QMainWindow *BoiterieClass)
    {
        BoiterieClass->setWindowTitle(QApplication::translate("BoiterieClass", "Boiterie", 0));
    } // retranslateUi

};

namespace Ui {
    class BoiterieClass: public Ui_BoiterieClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOITERIE_H
