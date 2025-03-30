#include "userwidget.h"
#include "dbmanager.h"
#include <QMessageBox>
#include <QSqlError>
#include <QInputDialog>
#include <QCryptographicHash>
#include "user.h"
#include <QRandomGenerator>
#include <QComboBox>

#include<QRegularExpression>
#include"rolemanagementdialog.h"

UserWidget::UserWidget(QWidget *parent)
    : QWidget(parent), model(nullptr), view(nullptr), btnAdd(nullptr)
{
    // 检查数据库连接
    if (!QSqlDatabase::database("doc_management_connection").isOpen()) {
        QMessageBox::critical(this, tr("错误"), tr("数据库未连接"));
        return;
    }

    setupUI();
    loadUserData();

}

void UserWidget::setupUI()
{
    // ================= 布局初始化 =================
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // ================= 表格视图配置 =================
    view = new QTableView(this);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); // 初始不可编辑

    // 使用命名数据库连接初始化模型
    model = new QSqlRelationalTableModel(this, QSqlDatabase::database("doc_management_connection"));
    model->setTable("users");
    model->setRelation(4, QSqlRelation("roles", "role_id", "role_name"));
    model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(4, Qt::Horizontal, tr("角色"));


    // 设置视图模型
    view->setModel(model);
    view->setColumnHidden(0, true);  // 隐藏user_id
    view->setColumnHidden(2, true);  // 隐藏password_hash
    view->setColumnHidden(3, true);  // 隐藏salt
    view->horizontalHeader()->setStretchLastSection(true);

    model->select();

    QLineEdit *searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("搜索用户"));
    connect(searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        model->setFilter(QString("username LIKE '%%1%'").arg(text));
        model->select();
    });
    mainLayout->addWidget(searchEdit);


    // ================= 权限控制按钮 =================
    QHBoxLayout *btnLayout = new QHBoxLayout;
    mainLayout->addLayout(btnLayout);
    if(UserManager::instance().currentUser()->hasPermission("manage_users")) {
        btnAdd = new QPushButton(tr("添加用户"), this);
        QPushButton *btnEdit = new QPushButton(tr("编辑用户"), this); // 新增
        QPushButton *btnDelete = new QPushButton(tr("删除用户"), this);

        connect(btnAdd, &QPushButton::clicked, this, &UserWidget::openAddUserDialog);
        connect(btnEdit, &QPushButton::clicked, this, &UserWidget::openEditUserDialog); // 新增
        connect(btnDelete, &QPushButton::clicked, this, [this](){ deleteSelectedUser(); });

        btnLayout->addWidget(btnAdd);
        btnLayout->addWidget(btnEdit); // 新增
        btnLayout->addWidget(btnDelete);
    }


    mainLayout->addWidget(view);
    // 添加角色管理按钮（管理员可见）
    if(UserManager::instance().currentUser()->hasPermission("manage_roles")) {
        QPushButton *btnManageRoles = new QPushButton(tr("角色管理"), this);
        connect(btnManageRoles, &QPushButton::clicked, this, &UserWidget::openRoleManagement);
        btnLayout->addWidget(btnManageRoles);
    }
    // mainLayout->addLayout(btnLayout);
    this->setLayout(mainLayout);


    if (UserManager::instance().currentUser()->hasPermission("manage_documents")) {
        QPushButton *btnDocManage = new QPushButton("批文管理", this);
        connect(btnDocManage, &QPushButton::clicked, [this]() {
            emit requestSwitchToDocument(); // 需在UserWidget.h声明信号
        });
        btnLayout->addWidget(btnDocManage);
    }
}

void UserWidget::openRoleManagement() {
    //角色管理界面逻辑
    RoleManagementDialog dialog(this);
    dialog.setWindowTitle("角色管理");
    dialog.exec(); // 显示模态对话框
    model->select(); // 刷新用户数据（如果角色影响用户显示）

}

//添加用户对话框
void UserWidget::openAddUserDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加用户");
    QFormLayout form(&dialog);

    // 用户名输入框
    QLineEdit *usernameEdit = new QLineEdit;
    form.addRow(tr("用户名:"), usernameEdit);

    // 密码输入框
    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    form.addRow(tr("密码:"), passwordEdit);

    // 角色选择下拉框（从数据库加载）
    QComboBox *roleCombo = new QComboBox;
    QSqlQuery roleQuery(QSqlDatabase::database("doc_management_connection"));
    roleQuery.prepare("SELECT role_id, role_name FROM roles");
    if (!roleQuery.exec()) {
        QMessageBox::critical(this, tr("错误"), tr("角色列表加载失败: ") + roleQuery.lastError().text());
        return;
    }

    // 填充角色数据
    if (roleQuery.size() == 0) {
        QMessageBox::warning(this, tr("警告"), tr("角色列表为空"));
    } else {
        while (roleQuery.next()) {
            QString roleName = roleQuery.value("role_name").toString();
            int roleId = roleQuery.value("role_id").toInt();
            roleCombo->addItem(roleName, roleId);
        }
    }
    form.addRow(tr("角色:"), roleCombo);

    // 确定/取消按钮
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString username = usernameEdit->text().trimmed();
        QString password = passwordEdit->text();
        int roleId = roleCombo->currentData().toInt();

        // 输入验证
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, tr("错误"), tr("用户名和密码不能为空"));
            return;
        }

        if (password.length() < 6 || !password.contains(QRegularExpression("[A-Za-z]"))) {
            QMessageBox::warning(this, tr("错误"), tr("密码必须至少6位且包含字母和数字"));
            return;
        }

        QSqlQuery checkQuery(QSqlDatabase::database("doc_management_connection"));
        checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
        checkQuery.addBindValue(username);
        if (!checkQuery.exec() || !checkQuery.next()) {
            QMessageBox::critical(this, tr("错误"), tr("用户名检查失败: ") + checkQuery.lastError().text());
            return;
        }
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, tr("错误"), tr("用户名已存在"));
            return;
        }

        // 密码加密
        QString salt = UserManager::instance().generateSalt();
        QString hashedPw = UserManager::instance().hashPassword(password, salt);

        // 数据库事务
        QSqlDatabase::database().transaction();

        QSqlQuery query(QSqlDatabase::database("doc_management_connection"));
        query.prepare("INSERT INTO users (username, password_hash, salt, role_id) VALUES (?, ?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(hashedPw);
        query.addBindValue(salt);
        query.addBindValue(roleId);

        if (!query.exec()) {
            QSqlDatabase::database().rollback();
            QMessageBox::critical(this, tr("错误"), tr("添加用户失败: ") + query.lastError().text());
        } else {
            QSqlDatabase::database().commit();
            model->select();

            // 定位到新用户
            QModelIndex newIndex = model->index(model->rowCount()-1, 1);
            view->scrollTo(newIndex);
            view->setCurrentIndex(newIndex);

            QMessageBox::information(this, tr("成功"), tr("用户添加成功"));
        }
    }
}

// 新增编辑用户对话框逻辑
void UserWidget::openEditUserDialog() {
    QModelIndexList selected = view->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("请先选择一个用户"));
        return;
    }

    int row = selected.first().row();
    int userId = model->data(model->index(row, 0)).toInt();
    QString username = model->data(model->index(row, 1)).toString();
    int roleId = model->data(model->index(row, 4)).toInt();

    QDialog dialog(this);
    dialog.setWindowTitle("编辑用户");
    QFormLayout form(&dialog);

    QLineEdit *usernameEdit = new QLineEdit(username);
    form.addRow(tr("用户名:"), usernameEdit);

    QComboBox *roleCombo = new QComboBox;
    QSqlQuery roleQuery(QSqlDatabase::database("doc_management_connection"));
    roleQuery.exec("SELECT role_id, role_name FROM roles");
    while (roleQuery.next()) {
        roleCombo->addItem(roleQuery.value("role_name").toString(), roleQuery.value("role_id"));
    }
    roleCombo->setCurrentIndex(roleCombo->findData(roleId));
    form.addRow(tr("角色:"), roleCombo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString newUsername = usernameEdit->text().trimmed();
        int newRoleId = roleCombo->currentData().toInt();

        QSqlQuery query(QSqlDatabase::database("doc_management_connection"));
        query.prepare("UPDATE users SET username = ?, role_id = ? WHERE user_id = ?");
        query.addBindValue(newUsername);
        query.addBindValue(newRoleId);
        query.addBindValue(userId);
        if (query.exec()) {
            model->select();
        } else {
            QMessageBox::critical(this, tr("错误"), tr("更新失败: ") + query.lastError().text());
        }
    }
}

//删除选择用户
void UserWidget::deleteSelectedUser()
{
    QModelIndexList selected = view->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择要删除的用户"));
        return;
    }

    int row = selected.first().row();
    int userId = model->data(model->index(row, 0)).toInt();

    if(QMessageBox::question(this, tr("确认"),
                              tr("确定要删除用户 %1 吗？").arg(model->data(model->index(row, 1)).toString()),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QSqlQuery query(QSqlDatabase::database("doc_management_connection"));
        query.prepare("DELETE FROM users WHERE user_id = ?");
        query.addBindValue(userId);

        if(!query.exec()) {
            QMessageBox::critical(this, tr("错误"),
                                  tr("删除失败: ") + query.lastError().text());
        } else {
            model->select();
        }
    }
}


void UserWidget::loadUserData() {
    qDebug() << "Loading user data...";
    if (!model->select()) {
        qDebug() << "[ERROR] Failed to load user data. Details:";
        qDebug() << "  - SQL Error:" << model->lastError().text();
        qDebug() << "  - Executed Query:" << model->query().lastQuery();
        QMessageBox::critical(this, tr("错误"), tr("无法加载用户数据: ") + model->lastError().text());
    } else {
        qDebug() << "User data loaded successfully.";
    }
}

// 盐值生成
QString UserManager::generateSalt() const {
    QByteArray saltBytes;
    saltBytes.resize(16); // 16字节盐值
    QRandomGenerator::global()->fillRange(reinterpret_cast<quint32*>(saltBytes.data()), saltBytes.size()/4);
    return QString(saltBytes.toBase64());
}
