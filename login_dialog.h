#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H
#include <QDialog>
#include "dbmanager.h"

namespace Ui {
class login_Dialog;
}

class login_Dialog : public QDialog
{
    Q_OBJECT
public:
    // 将枚举定义在类内部
    enum UserRole {
        Admin = 1,
        Author = 2,
        Other = 3
    };
    Q_ENUM(UserRole)  // 启用元对象特性

    explicit login_Dialog(QWidget *parent = nullptr);
    ~login_Dialog();

private slots: 
    void on_login_btn_clicked();
    void on_toolButton_toggled(bool checked);

    void on_login_btn_clicked(bool checked);

private:
    Ui::login_Dialog *ui;
    void showMainWindow(UserRole role);
};
#endif
