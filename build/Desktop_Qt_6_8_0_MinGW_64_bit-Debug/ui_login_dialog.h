/********************************************************************************
** Form generated from reading UI file 'login_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login_Dialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *user;
    QLabel *label_psw;
    QLabel *label_user;
    QToolButton *toolButton;
    QLineEdit *password;
    QPushButton *login_btn;

    void setupUi(QDialog *login_Dialog)
    {
        if (login_Dialog->objectName().isEmpty())
            login_Dialog->setObjectName("login_Dialog");
        login_Dialog->setEnabled(true);
        login_Dialog->resize(443, 300);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(login_Dialog->sizePolicy().hasHeightForWidth());
        login_Dialog->setSizePolicy(sizePolicy);
        login_Dialog->setMouseTracking(false);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::AddressBookNew));
        login_Dialog->setWindowIcon(icon);
        login_Dialog->setSizeGripEnabled(false);
        gridLayoutWidget = new QWidget(login_Dialog);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(120, 70, 201, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        user = new QLineEdit(gridLayoutWidget);
        user->setObjectName("user");

        gridLayout->addWidget(user, 1, 2, 1, 1);

        label_psw = new QLabel(gridLayoutWidget);
        label_psw->setObjectName("label_psw");

        gridLayout->addWidget(label_psw, 3, 0, 1, 1);

        label_user = new QLabel(gridLayoutWidget);
        label_user->setObjectName("label_user");

        gridLayout->addWidget(label_user, 1, 0, 1, 1);

        toolButton = new QToolButton(gridLayoutWidget);
        toolButton->setObjectName("toolButton");

        gridLayout->addWidget(toolButton, 3, 3, 1, 1);

        password = new QLineEdit(gridLayoutWidget);
        password->setObjectName("password");

        gridLayout->addWidget(password, 3, 2, 1, 1);

        login_btn = new QPushButton(login_Dialog);
        login_btn->setObjectName("login_btn");
        login_btn->setGeometry(QRect(160, 170, 141, 18));
        QWidget::setTabOrder(user, password);
        QWidget::setTabOrder(password, toolButton);
        QWidget::setTabOrder(toolButton, login_btn);

        retranslateUi(login_Dialog);

        QMetaObject::connectSlotsByName(login_Dialog);
    } // setupUi

    void retranslateUi(QDialog *login_Dialog)
    {
        login_Dialog->setWindowTitle(QCoreApplication::translate("login_Dialog", "Dialog", nullptr));
        label_psw->setText(QCoreApplication::translate("login_Dialog", "\345\257\206\347\240\201:", nullptr));
        label_user->setText(QCoreApplication::translate("login_Dialog", "\347\224\250\346\210\267\345\220\215:", nullptr));
        toolButton->setText(QString());
        login_btn->setText(QCoreApplication::translate("login_Dialog", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login_Dialog: public Ui_login_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
