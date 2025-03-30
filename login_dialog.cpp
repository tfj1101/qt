#include "login_dialog.h"
#include "ui_login_dialog.h"
#include<QToolButton>
#include"userwidget.h"
#include<user.h>
#include<dbmanager.h>
#include<QDebug>
#include"mainwindow.h"
#include<QMessageBox>

login_Dialog::login_Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::login_Dialog)
{
    ui->setupUi(this);
    setWindowTitle("登录界面");
    setFixedSize(443, 300);

    // 眼睛按钮初始化
    ui->toolButton->setCheckable(true);
    ui->toolButton->setChecked(true);
    ui->toolButton->setIcon(QIcon("D:/Qt/qt_project/piwen_systm/res/eye.png"));

    // 信号连接
    // connect(ui->login_btn, &QPushButton::clicked, this, &login_Dialog::on_login_btn_clicked);
    connect(ui->toolButton, &QToolButton::toggled, this, &login_Dialog::on_toolButton_toggled);
    
    // 数据库初始化
}

void login_Dialog::on_toolButton_toggled(bool checked)
{
    ui->password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    ui->toolButton->setIcon(QIcon(checked ? "D:\\Qt\\qt_project\\piwen_systm\\res\\eye.png" : "D:\\Qt\\qt_project\\piwen_systm\\res\\eye-close.png"));
}

login_Dialog::~login_Dialog()
{
    delete ui;
}


void login_Dialog::showMainWindow(UserRole role) {
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时自动删除
    // if (role == UserRole::Admin) {
    //     UserWidget *userWidget = new UserWidget();
    //     mainWindow->setCentralWidget(userWidget);
    //     // 删除 setParent 调用（setCentralWidget 已自动设置父对象）
    // } else if(role == UserRole::Author) {
    //     QLabel *welcomeLabel = new QLabel("欢迎使用批文管理系统（审批员）");
    //     welcomeLabel->setAlignment(Qt::AlignCenter);
    //     mainWindow->setCentralWidget(welcomeLabel);
    // }else {
    //     QLabel *welcomeLabel = new QLabel("欢迎使用批文管理系统（普通用户）");
    //     welcomeLabel->setAlignment(Qt::AlignCenter);
    //     mainWindow->setCentralWidget(welcomeLabel);
    // }
    // 根据角色设置初始页面
    if (role == UserRole::Admin) {
        mainWindow->switchToUserManagement();
    } else {
        mainWindow->switchToDocumentManagement();
    }
    mainWindow->show();
    this->close(); // 关闭登录对话框
}

//登录按钮事件
void login_Dialog::on_login_btn_clicked() {
    ui->login_btn->setEnabled(false);
    QString username = ui->user->text();
    QString password = ui->password->text();

    DBManager& dbm = DBManager::instance();

    if (!dbm.connect()) {
        QMessageBox::critical(this, "错误", "数据库连接失败: " + dbm.lastError().text());
            ui->login_btn->setEnabled(false);
        return;
    }

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名和密码不能为空！");
        ui->login_btn->setEnabled(true);  // 提前返回前恢复按钮
        return;
    }

    // 调用 UserManager 的登录方法
    if (UserManager::instance().login(username, password)) {
        if (User* user = UserManager::instance().currentUser()) { // 空指针检查
            qDebug() << "登录成功！用户角色ID：" << user->roleId();
            showMainWindow(static_cast<UserRole>(user->roleId())); // 显式类型转换
        } else {
            QMessageBox::warning(this, "错误", "用户数据异常");
        }
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    }
    // ui->login_btn->setEnabled(true); // 最终恢复按钮
}


void login_Dialog::on_login_btn_clicked(bool checked)
{
    if (checked) {
        ui->login_btn->setEnabled(false);
        QString username = ui->user->text();
        QString password = ui->password->text();

        DBManager& dbm = DBManager::instance();

        if (!dbm.connect()) {
            QMessageBox::critical(this, "错误", "数据库连接失败: " + dbm.lastError().text());
            ui->login_btn->setEnabled(false);
            return;
        }

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "错误", "用户名和密码不能为空！");
            ui->login_btn->setEnabled(true);  // 提前返回前恢复按钮
            return;
        }

        // 调用 UserManager 的登录方法
        if (UserManager::instance().login(username, password)) {
            if (User* user = UserManager::instance().currentUser()) { // 空指针检查
                qDebug() << "登录成功！用户角色ID：" << user->roleId();
                showMainWindow(static_cast<UserRole>(user->roleId())); // 显式类型转换
            } else {
                QMessageBox::warning(this, "错误", "用户数据异常");
            }
        } else {
            QMessageBox::warning(this, "错误", "用户名或密码错误");
        }
    }
}

