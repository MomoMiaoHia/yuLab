/********************************************************************************
** Form generated from reading UI file 'yuLab.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YULAB_H
#define UI_YULAB_H

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

class Ui_yuLabClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *yuLabClass)
    {
        if (yuLabClass->objectName().isEmpty())
            yuLabClass->setObjectName(QStringLiteral("yuLabClass"));
        yuLabClass->resize(600, 400);
        menuBar = new QMenuBar(yuLabClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        yuLabClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(yuLabClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        yuLabClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(yuLabClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        yuLabClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(yuLabClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        yuLabClass->setStatusBar(statusBar);

        retranslateUi(yuLabClass);

        QMetaObject::connectSlotsByName(yuLabClass);
    } // setupUi

    void retranslateUi(QMainWindow *yuLabClass)
    {
        yuLabClass->setWindowTitle(QApplication::translate("yuLabClass", "yuLab", 0));
    } // retranslateUi

};

namespace Ui {
    class yuLabClass: public Ui_yuLabClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YULAB_H
