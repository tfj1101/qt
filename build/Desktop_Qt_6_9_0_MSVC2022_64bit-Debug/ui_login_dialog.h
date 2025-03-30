/********************************************************************************
** Form generated from reading UI file 'login_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login_Dialog
{
public:
    QPushButton *login_btn;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_user;
    QLineEdit *user;
    QLabel *label_psw;
    QLineEdit *password;
    QToolButton *toolButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *admin;
    QRadioButton *other;

    void setupUi(QDialog *login_Dialog)
    {
        if (login_Dialog->objectName().isEmpty())
            login_Dialog->setObjectName("login_Dialog");
        login_Dialog->resize(443, 300);
        login_btn = new QPushButton(login_Dialog);
        login_btn->setObjectName("login_btn");
        login_btn->setGeometry(QRect(180, 240, 80, 18));
        gridLayoutWidget = new QWidget(login_Dialog);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(120, 70, 201, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_user = new QLabel(gridLayoutWidget);
        label_user->setObjectName("label_user");

        gridLayout->addWidget(label_user, 1, 0, 1, 1);

        user = new QLineEdit(gridLayoutWidget);
        user->setObjectName("user");

        gridLayout->addWidget(user, 1, 2, 1, 1);

        label_psw = new QLabel(gridLayoutWidget);
        label_psw->setObjectName("label_psw");

        gridLayout->addWidget(label_psw, 3, 0, 1, 1);

        password = new QLineEdit(gridLayoutWidget);
        password->setObjectName("password");

        gridLayout->addWidget(password, 3, 2, 1, 1);

        toolButton = new QToolButton(gridLayoutWidget);
        toolButton->setObjectName("toolButton");

        gridLayout->addWidget(toolButton, 3, 3, 1, 1);

        horizontalLayoutWidget = new QWidget(login_Dialog);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(120, 150, 191, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        admin = new QRadioButton(horizontalLayoutWidget);
        admin->setObjectName("admin");

        horizontalLayout->addWidget(admin);

        other = new QRadioButton(horizontalLayoutWidget);
        other->setObjectName("other");

        horizontalLayout->addWidget(other);


        retranslateUi(login_Dialog);

        QMetaObject::connectSlotsByName(login_Dialog);
    } // setupUi

    void retranslateUi(QDialog *login_Dialog)
    {
        login_Dialog->setWindowTitle(QCoreApplication::translate("login_Dialog", "Dialog", nullptr));
        login_btn->setText(QCoreApplication::translate("login_Dialog", "\347\231\273\345\275\225", nullptr));
        label_user->setText(QCoreApplication::translate("login_Dialog", "\347\224\250\346\210\267\345\220\215:", nullptr));
        label_psw->setText(QCoreApplication::translate("login_Dialog", "\345\257\206\347\240\201:", nullptr));
        toolButton->setText(QCoreApplication::translate("login_Dialog", "...", nullptr));
        admin->setText(QCoreApplication::translate("login_Dialog", "\347\256\241\347\220\206\345\221\230", nullptr));
        other->setText(QCoreApplication::translate("login_Dialog", "\346\231\256\351\200\232\347\224\250\346\210\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login_Dialog: public Ui_login_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
