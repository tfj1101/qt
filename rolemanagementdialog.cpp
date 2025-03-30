
#include "rolemanagementdialog.h"
#include "qlabel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <QInputDialog>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include<QCheckBox>
#include<QJsonDocument>
#include<QFile>

// 在构造函数中添加视图选择信号连接
RoleManagementDialog::RoleManagementDialog(QWidget *parent) : QDialog(parent), model(nullptr), view(nullptr)
{
    // 初始化模型
    model = new QSqlRelationalTableModel(this, QSqlDatabase::database("doc_management_connection"));
    model->setTable("roles");
    model->setHeaderData(1, Qt::Horizontal, tr("角色名称"));
    model->select();

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 角色列表
    view = new QTableView(this);
    view->setModel(model);
    view->setColumnHidden(0, true); // 隐藏role_id
    mainLayout->addWidget(view);

    // 权限配置区域
    setupPermissionsUI();

    // 操作按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *btnAdd = new QPushButton(tr("添加角色"), this);
    QPushButton *btnEdit = new QPushButton(tr("编辑角色"), this);
    QPushButton *btnDelete = new QPushButton(tr("删除角色"), this);
    QPushButton *btnSave = new QPushButton(tr("保存权限"), this);

    connect(btnAdd, &QPushButton::clicked, this, &RoleManagementDialog::addRole);
    connect(btnEdit, &QPushButton::clicked, this, &RoleManagementDialog::editRole);
    connect(btnDelete, &QPushButton::clicked, this, &RoleManagementDialog::deleteRole);
    connect(btnSave, &QPushButton::clicked, this, &RoleManagementDialog::savePermissions);

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addWidget(btnSave);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
    
    // 添加角色选择信号连接
    connect(view->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &RoleManagementDialog::onRoleSelectionChanged);
}

void RoleManagementDialog::addRole() {
    QString roleName = QInputDialog::getText(this, tr("添加角色"), tr("角色名称:"));
    if (!roleName.isEmpty()) {
        QSqlQuery query;
        query.prepare("INSERT INTO roles (role_name) VALUES (?)");
        query.addBindValue(roleName);
        if (query.exec()) {
            model->select();
            QMessageBox::information(this, tr("成功"), tr("角色添加成功"));  // 新增成功提示
        } else {
            QMessageBox::critical(this, tr("错误"), tr("添加失败: ") + query.lastError().text());
        }
    }
}

void RoleManagementDialog::editRole() {
    QModelIndex index = view->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择要编辑的角色"));  // 添加选择检查
        return;
    }

    QString oldName = model->data(model->index(index.row(), 1)).toString();
    QString newName = QInputDialog::getText(this, tr("编辑角色"), tr("新名称:"), QLineEdit::Normal, oldName);
    if (!newName.isEmpty()) {
        model->setData(model->index(index.row(), 1), newName);
        if (!model->submitAll()) {  // 添加提交结果检查
            QMessageBox::critical(this, tr("错误"), tr("提交失败: ") + model->lastError().text());
        }
    }
}

void RoleManagementDialog::deleteRole() {
    QModelIndexList selected = view->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择一个角色"));
        return;
    }
    int row = selected.first().row();
    if (QMessageBox::question(this, tr("确认"), tr("确定删除该角色吗？")) == QMessageBox::Yes) {
        model->removeRow(row);
        model->submitAll();
    }
}

void RoleManagementDialog::setupPermissionsUI() {
    QGroupBox *permissionGroup = new QGroupBox(tr("权限配置"), this);
    QVBoxLayout *permissionLayout = new QVBoxLayout(permissionGroup);

    // 创建权限说明的显示区域
    QLabel *permissionDescription = new QLabel(this);
    permissionDescription->setWordWrap(true);
    permissionDescription->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    permissionDescription->setStyleSheet("QLabel { color: #666; font-style: italic; }");
    permissionDescription->setMinimumHeight(60);

    // 将说明标签添加到布局
    permissionLayout->addWidget(permissionDescription);

    QFile file("D:\\Qt\\qt_project\\piwen_systm\\config\\permissions.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            QJsonObject permissions = doc.object();

            // 创建权限复选框
            for (auto it = permissions.begin(); it != permissions.end(); ++it) {
                QString permKey = it.key();
                QString permDesc = it.value().toString();

                // 创建带有详细说明的复选框
                QCheckBox *checkBox = new QCheckBox(permKey, permissionGroup);

                // 设置工具提示（鼠标悬停时显示）
                checkBox->setToolTip(permDesc);

                // 设置状态提示（状态栏显示）
                checkBox->setStatusTip(permDesc);

                // 连接信号，当复选框被选中时更新说明
                connect(checkBox, &QCheckBox::clicked, this, [=](bool checked){
                    QString desc = QString("<b>%1</b><br>%2").arg(permKey).arg(permDesc);
                    if(checked) {
                        desc += "<br><font color='green'>✓ 已启用</font>";
                    } else {
                        desc += "<br><font color='gray'>✗ 未启用</font>";
                    }
                    permissionDescription->setText(desc);
                });

                // 连接鼠标悬停信号
                connect(checkBox, &QCheckBox::pressed, this, [=](){
                    permissionDescription->setText(QString("<b>%1</b><br>%2").arg(permKey).arg(permDesc));
                });

                permissionCheckboxes.insert(permKey, checkBox);
                permissionLayout->addWidget(checkBox);
            }
        } else {
            QMessageBox::warning(this, tr("错误"), tr("权限配置文件解析失败: ") + parseError.errorString());
            setupDefaultPermissions(permissionLayout, permissionDescription);
        }
        file.close();
    } else {
        QMessageBox::warning(this, tr("错误"), tr("无法打开权限配置文件"));
        setupDefaultPermissions(permissionLayout, permissionDescription);
    }

    permissionGroup->setLayout(permissionLayout);

    // 添加到主布局
    if (QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout())) {
        mainLayout->addWidget(permissionGroup);
    } else {
        QVBoxLayout *newMainLayout = new QVBoxLayout(this);
        newMainLayout->addWidget(permissionGroup);
        this->setLayout(newMainLayout);
    }
}

void RoleManagementDialog::setupDefaultPermissions(QVBoxLayout *layout, QLabel *descLabel) {
    // 默认权限集合
    QHash<QString, QString> defaultPermissions = {
        {"manage_users", "允许添加、编辑和删除系统用户"},
        {"manage_roles", "允许管理角色和权限分配"},
        {"submit_documents", "允许提交新的批文申请"},
        {"approve_documents", "允许审批待处理的批文"},
        {"view_all_documents", "允许查看所有批文，包括非自己创建的"}
    };

    for (auto it = defaultPermissions.begin(); it != defaultPermissions.end(); ++it) {
        QString permKey = it.key();
        QString permDesc = it.value();

        QCheckBox *checkBox = new QCheckBox(permKey, this);
        checkBox->setToolTip(permDesc);
        checkBox->setStatusTip(permDesc);

        connect(checkBox, &QCheckBox::clicked, this, [=](bool checked){
            QString desc = QString("<b>%1</b><br>%2").arg(permKey).arg(permDesc);
            if(checked) {
                desc += "<br><font color='green'>✓ 已启用</font>";
            } else {
                desc += "<br><font color='gray'>✗ 未启用</font>";
            }
            descLabel->setText(desc);
        });

        connect(checkBox, &QCheckBox::pressed, this, [=](){
            descLabel->setText(QString("<b>%1</b><br>%2").arg(permKey).arg(permDesc));
        });

        permissionCheckboxes.insert(permKey, checkBox);
        layout->addWidget(checkBox);
    }

    // 设置初始说明
    descLabel->setText(tr("<i>将鼠标悬停在权限上查看详细说明</i>"));
}

void RoleManagementDialog::savePermissions() {
    QModelIndex index = view->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择要配置的角色"));
        return;
    }

    int roleId = model->data(model->index(index.row(), 0)).toInt();
    QJsonObject permissions;
    for (auto it = permissionCheckboxes.begin(); it != permissionCheckboxes.end(); ++it) {
        permissions[it.key()] = it.value()->isChecked();
    }

    QSqlQuery query;
    query.prepare("UPDATE roles SET permissions = ? WHERE role_id = ?");
    query.addBindValue(QJsonDocument(permissions).toJson());
    query.addBindValue(roleId);
    if (query.exec()) {
        QMessageBox::information(this, tr("成功"), tr("权限保存成功"));
    } else {
        QMessageBox::critical(this, tr("错误"), tr("保存权限失败: ") + query.lastError().text());
    }
    if (permissions.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("未选择任何权限"));
        return;
    }
}

// 实现槽函数
void RoleManagementDialog::onRoleSelectionChanged() {
    qDebug() << "角色选择已变更";
}
