/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *buttonLayout;
    QPushButton *btnAdd;
    QPushButton *btnDelete;

    void setupUi(QWidget *UserWidget)
    {
        if (UserWidget->objectName().isEmpty())
            UserWidget->setObjectName("UserWidget");
        verticalLayout = new QVBoxLayout(UserWidget);
        verticalLayout->setObjectName("verticalLayout");
        tableView = new QTableView(UserWidget);
        tableView->setObjectName("tableView");
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(tableView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        btnAdd = new QPushButton(UserWidget);
        btnAdd->setObjectName("btnAdd");

        buttonLayout->addWidget(btnAdd);

        btnDelete = new QPushButton(UserWidget);
        btnDelete->setObjectName("btnDelete");

        buttonLayout->addWidget(btnDelete);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(UserWidget);

        QMetaObject::connectSlotsByName(UserWidget);
    } // setupUi

    void retranslateUi(QWidget *UserWidget)
    {
        btnAdd->setText(QCoreApplication::translate("UserWidget", "\346\267\273\345\212\240\347\224\250\346\210\267", nullptr));
        btnDelete->setText(QCoreApplication::translate("UserWidget", "\345\210\240\351\231\244\347\224\250\346\210\267", nullptr));
        (void)UserWidget;
    } // retranslateUi

};

namespace Ui {
    class UserWidget: public Ui_UserWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
