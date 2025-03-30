#include "mainwindow.h"
#include "login_dialog.h"
#include "DocumentWidget.h"
#include <QApplication>
#include<QSqlDatabase>
#include<QMessageBox>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 检查 MySQL 驱动是否可用
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        qDebug() << "QMYSQL driver not available!";
        return -1;
    }

    // 测试数据库连接
    DBManager& dbManager = DBManager::instance();
    if (!dbManager.connect()) {
        qDebug() << "Database connection failed:" << dbManager.lastError().text();
        QMessageBox::critical(nullptr, "错误", "数据库连接失败:" + dbManager.lastError().text());
        return -1;
    }

    login_Dialog login;
    login.show();


    return a.exec();
}
