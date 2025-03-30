#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlTableModel>
#include<QSqlRelationalTableModel>
#include<QTableView>
#include"dbmanager.h"
#include <QMenuBar>
#include <QAction>
#include<QMessageBox>
#include"ApprovalWorkflow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化数据库连接（确保在程序入口处仅一次）
    DBManager& db = DBManager::instance();
    if (!db.connect()) {
        QMessageBox::critical(this, "错误", "数据库连接失败");
        exit(1);
    }
    setWindowTitle("批文管理系统");
    // 先初始化模块组件
    // userWidget = new UserWidget();
    // docWidget = new DocumentWidget();  // ✅ 先创建docWidget


    


    // 创建堆栈布局容器
    stackedWidget = new QStackedWidget(this);


    // 初始化各功能模块
    userWidget = new UserWidget();
    docWidget = new DocumentWidget();

    if (!userWidget || !docWidget) {
        QMessageBox::critical(this, "错误", "UI组件初始化失败");
        exit(1);
    }
    stackedWidget->addWidget(userWidget);
    stackedWidget->addWidget(docWidget);
    setCentralWidget(stackedWidget);


    // 后建立信号连接
    connect(&ApprovalWorkflow::instance(), &ApprovalWorkflow::approvalProcessed,
            docWidget, &DocumentWidget::onApprovalProcessed);

    // 默认显示用户管理界面
    switchToUserManagement();

    // 设置菜单栏
    setupMenuBar();

}

void MainWindow::setupMenuBar() {
    QMenu *menu = menuBar()->addMenu("功能导航");

    QAction *actionUser = new QAction("用户管理", this);
    QAction *actionDoc = new QAction("批文管理", this);

    menu->addAction(actionUser);
    menu->addAction(actionDoc);

    connect(actionUser, &QAction::triggered, this, &MainWindow::switchToUserManagement);
    connect(actionDoc, &QAction::triggered, this, &MainWindow::switchToDocumentManagement);


}

void MainWindow::switchToUserManagement() {
    stackedWidget->setCurrentWidget(userWidget);
}

void MainWindow::switchToDocumentManagement() {
    stackedWidget->setCurrentWidget(docWidget);
}

// 添加头文件包含检查
#ifndef APPROVALWORKFLOW_H
#include "ApprovalWorkflow.h"
#endif

// 在析构函数中添加资源释放
MainWindow::~MainWindow()
{
    delete ui;
    // delete userWidget;  // 确保释放内存
    // delete docWidget;
}
