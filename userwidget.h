#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QSqlRelation>
#include <QSqlQuery>
// 前置声明减少编译依赖
class QModelIndex;
class QComboBox;

/**
 * @brief 用户管理组件，提供用户列表展示及增删功能
 *
 * 功能特性：
 * - 显示用户列表（关联角色名称）
 * - 管理员权限控制（显示操作按钮）
 * - 支持添加/删除用户
 * - 自动隐藏敏感字段（密码哈希、盐值）
 */
class UserWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化界面和数据
     * @param parent 父组件指针
     */
    explicit UserWidget(QWidget *parent = nullptr);



private slots:
    ///打开添加用户对话框
    void openAddUserDialog();

    /// 删除选中的用户
    void deleteSelectedUser();
    ///管理角色
    void openRoleManagement();
    ///用户编辑对话框
    void openEditUserDialog() ;

signals:
    void requestSwitchToDocument();

private:
    //初始化界面布局和组件
    void setupUI();

    //加载用户数据到表格
    void loadUserData();

    // 数据库模型组件
    QSqlRelationalTableModel *model = nullptr;  // 关联式数据模型（连接users和roles表）
    QTableView *view = nullptr;                 // 表格视图组件

    // 操作按钮（仅对有权限的用户可见）
    QPushButton *btnAdd = nullptr;    //< 添加用户按钮
    QPushButton *btnDelete = nullptr; //< 删除用户按钮

    // 布局组件
    QVBoxLayout *mainLayout = nullptr; ///< 主垂直布局
};

#endif // USERWIDGET_H
