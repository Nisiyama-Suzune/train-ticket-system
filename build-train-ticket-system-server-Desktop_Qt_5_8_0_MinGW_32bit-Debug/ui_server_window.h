/********************************************************************************
** Form generated from reading UI file 'server_window.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_WINDOW_H
#define UI_SERVER_WINDOW_H

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

class Ui_server_window
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *server_window)
    {
        if (server_window->objectName().isEmpty())
            server_window->setObjectName(QStringLiteral("server_window"));
        server_window->resize(400, 300);
        menuBar = new QMenuBar(server_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        server_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(server_window);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        server_window->addToolBar(mainToolBar);
        centralWidget = new QWidget(server_window);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        server_window->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(server_window);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        server_window->setStatusBar(statusBar);

        retranslateUi(server_window);

        QMetaObject::connectSlotsByName(server_window);
    } // setupUi

    void retranslateUi(QMainWindow *server_window)
    {
        server_window->setWindowTitle(QApplication::translate("server_window", "server_window", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class server_window: public Ui_server_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_WINDOW_H
