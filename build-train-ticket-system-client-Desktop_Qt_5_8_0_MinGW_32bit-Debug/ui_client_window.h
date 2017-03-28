/********************************************************************************
** Form generated from reading UI file 'client_window.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_WINDOW_H
#define UI_CLIENT_WINDOW_H

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

class Ui_client_window
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *client_window)
    {
        if (client_window->objectName().isEmpty())
            client_window->setObjectName(QStringLiteral("client_window"));
        client_window->resize(400, 300);
        menuBar = new QMenuBar(client_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        client_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(client_window);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        client_window->addToolBar(mainToolBar);
        centralWidget = new QWidget(client_window);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        client_window->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(client_window);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        client_window->setStatusBar(statusBar);

        retranslateUi(client_window);

        QMetaObject::connectSlotsByName(client_window);
    } // setupUi

    void retranslateUi(QMainWindow *client_window)
    {
        client_window->setWindowTitle(QApplication::translate("client_window", "client_window", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class client_window: public Ui_client_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_WINDOW_H
