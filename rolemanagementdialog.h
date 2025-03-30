// rolemanagementdialog.h
#ifndef ROLEMANAGEMENTDIALOG_H
#define ROLEMANAGEMENTDIALOG_H

#include "qboxlayout.h"
#include "qlabel.h"
#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QJsonObject>

class QTableView;
class QCheckBox;

class RoleManagementDialog : public QDialog {
    Q_OBJECT
public:
    explicit RoleManagementDialog(QWidget *parent = nullptr);

private slots:
    void addRole();
    void editRole();
    void deleteRole();
    void savePermissions();
    void onRoleSelectionChanged();
    // void setupDefaultPermissions(QVBoxLayout *layout);
    void setupDefaultPermissions(QVBoxLayout *layout, QLabel *descLabel);
private:
    void loadRoles();
    void setupPermissionsUI();

    QSqlRelationalTableModel *model;
    QTableView *view;
    QHash<QString, QCheckBox*> permissionCheckboxes; // 权限复选框

    void loadPermissionsForRole(int roleId);  // 新增权限加载方法
    QLabel *permissionDescription = nullptr;  // 新增权限说明标签

    // 常量定义
    const QHash<QString, QString> DEFAULT_PERMISSIONS = {
        {"manage_users", "允许添加、编辑和删除系统用户"},
        {"manage_roles", "允许管理角色和权限分配"},
        {"submit_documents", "允许提交新的批文申请"},
        {"approve_documents", "允许审批待处理的批文"},
        {"view_all_documents", "允许查看所有批文，包括非自己创建的"}
    };
};

#endif // ROLEMANAGEMENTDIALOG_H
