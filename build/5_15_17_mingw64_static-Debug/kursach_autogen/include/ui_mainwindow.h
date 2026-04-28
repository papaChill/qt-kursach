/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QAction *action_3;
    QAction *action_4;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QAction *action_8;
    QAction *action_9;
    QAction *action_10;
    QAction *action_11;
    QAction *action_12;
    QAction *action_13;
    QAction *action_14;
    QAction *action_15;
    QAction *action_16;
    QAction *action_17;
    QAction *action_18;
    QAction *action_19;
    QAction *action_20;
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QTableWidget *tableWidget;
    QGroupBox *groupBox_2;
    QTableWidget *tableWidget_2;
    QGroupBox *groupBox_3;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1305, 700);
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_4 = new QAction(MainWindow);
        action_4->setObjectName(QString::fromUtf8("action_4"));
        action_5 = new QAction(MainWindow);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(MainWindow);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_7 = new QAction(MainWindow);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        action_8 = new QAction(MainWindow);
        action_8->setObjectName(QString::fromUtf8("action_8"));
        action_9 = new QAction(MainWindow);
        action_9->setObjectName(QString::fromUtf8("action_9"));
        action_10 = new QAction(MainWindow);
        action_10->setObjectName(QString::fromUtf8("action_10"));
        action_11 = new QAction(MainWindow);
        action_11->setObjectName(QString::fromUtf8("action_11"));
        action_12 = new QAction(MainWindow);
        action_12->setObjectName(QString::fromUtf8("action_12"));
        action_13 = new QAction(MainWindow);
        action_13->setObjectName(QString::fromUtf8("action_13"));
        action_14 = new QAction(MainWindow);
        action_14->setObjectName(QString::fromUtf8("action_14"));
        action_15 = new QAction(MainWindow);
        action_15->setObjectName(QString::fromUtf8("action_15"));
        action_16 = new QAction(MainWindow);
        action_16->setObjectName(QString::fromUtf8("action_16"));
        action_17 = new QAction(MainWindow);
        action_17->setObjectName(QString::fromUtf8("action_17"));
        action_18 = new QAction(MainWindow);
        action_18->setObjectName(QString::fromUtf8("action_18"));
        action_19 = new QAction(MainWindow);
        action_19->setObjectName(QString::fromUtf8("action_19"));
        action_20 = new QAction(MainWindow);
        action_20->setObjectName(QString::fromUtf8("action_20"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(70, 30, 721, 291));
        tableWidget = new QTableWidget(groupBox);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 20, 701, 261));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(70, 330, 721, 291));
        tableWidget_2 = new QTableWidget(groupBox_2);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(10, 20, 701, 261));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(810, 30, 431, 591));
        textEdit = new QTextEdit(groupBox_3);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 20, 411, 561));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1305, 21));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(action);
        menu->addAction(action_2);
        menu->addAction(action_3);
        menu->addAction(action_4);
        menu->addAction(action_5);
        menu->addAction(action_6);
        menu->addAction(action_7);
        menu->addSeparator();
        menu_2->addAction(action_8);
        menu_2->addAction(action_9);
        menu_2->addAction(action_10);
        menu_2->addAction(action_11);
        menu_2->addAction(action_12);
        menu_2->addAction(action_13);
        menu_2->addAction(action_14);
        menu_3->addAction(action_15);
        menu_3->addAction(action_18);
        menu_3->addAction(action_19);
        menu_3->addAction(action_20);
        menu_4->addAction(action_16);
        menu_4->addAction(action_17);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        action_2->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        action_3->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270", nullptr));
        action_4->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\201\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272", nullptr));
        action_5->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\205\321\215\321\210-\321\202\320\260\320\261\320\273\320\270\321\206\321\203", nullptr));
        action_6->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        action_7->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\201 \321\204\320\260\320\271\320\273\320\260", nullptr));
        action_8->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        action_9->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        action_10->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        action_11->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\201\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272", nullptr));
        action_12->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \320\264\320\265\321\200\320\265\320\262\320\276", nullptr));
        action_13->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        action_14->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\201 \321\204\320\260\320\271\320\273\320\260", nullptr));
        action_15->setText(QCoreApplication::translate("MainWindow", "\320\241\321\204\320\276\321\200\320\274\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        action_16->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\220\320\222\320\233", nullptr));
        action_17->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \321\205\321\215\321\210\320\265\320\271", nullptr));
        action_18->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\264\320\265\321\200\320\265\320\262\320\276", nullptr));
        action_19->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214", nullptr));
        action_20->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262 \321\204\320\260\320\271\320\273", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\320\245\320\265\321\210-\320\242\320\260\320\261\320\273\320\270\321\206\320\260", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\320\220\320\262\320\273-\320\264\320\265\321\200\320\265\320\262\320\276", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\273\320\260\320\264\320\272\320\260", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272 \"\320\242\321\200\320\265\320\275\320\265\321\200\321\213\"", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272 \"\320\242\321\200\320\265\320\275\320\270\321\200\320\276\320\262\320\272\320\270\"", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\320\236\321\202\321\207\320\265\321\202", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\273\320\260\320\264\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
