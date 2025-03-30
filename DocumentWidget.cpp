#include "DocumentWidget.h"
#include "DocumentApplyDialog.h"
#include "StatusColorDelegate.h"
#include "DocumentDetailsDialog.h"
#include "ApprovalDialog.h"          // 新增审批对话框头文件
#include "WorkflowConfigDialog.h"
#include "user.h"
#include "dbmanager.h"
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlError>
#include <QDebug>
#include <QProgressDialog>
#include <QApplication>

// 在构造函数中移除冗余布局设置
DocumentWidget::DocumentWidget(QWidget *parent) : QWidget(parent) {
    // 初始化布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // 操作按钮布局
    btnLayout = new QHBoxLayout; // 使用成员变量
    btnApply = new QPushButton("新建批文", this);
    btnApprove = new QPushButton("待审批", this);
    btnLayout->addWidget(btnApply);
    btnLayout->addWidget(btnApprove);
    layout->addLayout(btnLayout);

    // 初始化数据库连接（通过DBManager单例动态配置）
    DBManager& db = DBManager::instance();
    if (!db.connect()) {  // 参数应在其他位置（如登录后）动态设置
        QMessageBox::critical(this, "错误", "数据库连接失败: " + db.lastError().text());
        exit(EXIT_FAILURE);
    }

    // 批文列表
    docTable = new QTableView(this);
    docTable->setSelectionMode(QAbstractItemView::SingleSelection);
    docTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(docTable);

    // 连接信号槽
    connect(btnApply, &QPushButton::clicked, this, &DocumentWidget::onApplyDocument);
    connect(btnApprove, &QPushButton::clicked, this, &DocumentWidget::onApproveDocument);
    connect(docTable, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        int docId = index.sibling(index.row(), 0).data().toInt();
        DocumentDetailsDialog dialog(docId, this);
        dialog.exec();
    });
    // 删除 this->setLayout(layout);
}

void DocumentWidget::checkPermissions() {
    User *user = UserManager::instance().currentUser();
    if (!user) {
        qCritical() << "当前用户未登录或无效";
        return;
    }

    // 移除 static 声明，使用成员变量
    if (user->hasPermission("manage_documents") && !btnConfig) {
        btnConfig = new QPushButton("流程配置", this);
        connect(btnConfig, &QPushButton::clicked, this, &DocumentWidget::openWorkflowConfig);
        btnLayout->addWidget(btnConfig);
    }
    // 普通用户：只能提交批文
    btnApply->setVisible(user->hasPermission("submit_documents"));

    // 审批员：可查看待审批列表
    btnApprove->setVisible(user->hasPermission("approve_documents"));

    // 管理员：动态添加流程配置按钮（避免重复创建）
    // 修改按钮配置逻辑（移除static，使用成员变量）
    if (user->hasPermission("manage_documents") && !this->btnConfig) {
        this->btnConfig = new QPushButton("流程配置", this);
        connect(btnConfig, &QPushButton::clicked, this, &DocumentWidget::openWorkflowConfig);
        btnLayout->addWidget(btnConfig);
    }
}

void DocumentWidget::onApplyDocument() {
    // 检查权限
    if (!UserManager::instance().currentUser()->hasPermission("submit_documents")) {
        QMessageBox::warning(this, "权限不足", "您没有提交批文的权限");
        return;
    }

    DocumentApplyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString title = dialog.getTitle().trimmed();
        QString content = dialog.getContent().trimmed();
        QString attachmentPath = dialog.getAttachmentPath();

        // 输入验证
        if (title.isEmpty()) {
            QMessageBox::warning(this, "错误", "标题不能为空");
            return;
        }

        if (content.isEmpty()) {
            QMessageBox::warning(this, "错误", "内容不能为空");
            return;
        }

        int applicantId = UserManager::instance().currentUser()->id();

        // 显示处理中提示
        QProgressDialog progress("正在提交批文...", "取消", 0, 0, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        QApplication::processEvents();

        // 提交文档
        bool success = DocumentManager::instance().submitDocument(
            title,
            content,
            applicantId,
            attachmentPath
            );

        progress.close();

        if (success) {
            QMessageBox::information(this, "成功", "批文提交成功");
            refreshDocumentList();
        } else {
            QMessageBox::critical(
                this,
                "错误",
                "提交失败: " + DocumentManager::instance().lastError().databaseText()
                );
        }
    }
}


// void DocumentWidget::onApplyDocument() {
//     DocumentApplyDialog dialog(this);
//     if (dialog.exec() == QDialog::Accepted) {
//         QString title = dialog.getTitle();
//         QString content = dialog.getContent();
//         QString attachmentPath = dialog.getAttachmentPath();  // 获取附件路径
//         int applicantId = UserManager::instance().currentUser()->id();

//         // 使用DocumentManager提交（包含附件路径）
//         DocumentManager::instance().submitDocument(title, content, applicantId, attachmentPath);
//         refreshDocumentList();
//     //     if (DocumentManager::instance().submitDocument(title, content, applicantId, attachmentPath)) {
//     //         refreshDocumentList();
//     //         QMessageBox::information(this, "成功", "批文提交成功！");
//     //     } else {
//     //         QMessageBox::critical(this, "错误", "提交失败: " + DocumentManager::instance().lastError());
//     //     }
//     }
// }

// void DocumentWidget::onApproveDocument() {
//     QModelIndexList selected = docTable->selectionModel()->selectedRows();
//     if (selected.isEmpty()) {
//         QMessageBox::warning(this, "警告", "请先选择一条批文");
//         return;
//     }

//     int docId = selected.first().data().toInt();
//     ApprovalDialog dialog(docId, this);  // 调用审批对话框
//     dialog.exec();
//     refreshDocumentList();
// }

void DocumentWidget::onApproveDocument() {
    // 1. 检查权限
    if (!UserManager::instance().currentUser()->hasPermission("approve_documents")) {
        QMessageBox::warning(this, "权限不足", "您没有审批权限");
        return;
    }

    // 2. 检查模型和选择
    if (!docTable->model() || docTable->selectionModel()->selectedRows().isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一条待审批的批文");
        return;
    }

    // 3. 安全获取文档ID
    QModelIndexList selected = docTable->selectionModel()->selectedRows(0); // 获取第0列(ID列)
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取批文ID");
        return;
    }

    // 4. 从模型获取原始数据
    int docId = docTable->model()->data(selected.first()).toInt();
    if (docId <= 0) {
        QMessageBox::critical(this, "错误", "无效的批文ID");
        return;
    }

    // 5. 打开审批对话框
    ApprovalDialog dialog(docId, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshDocumentList();
    }
}


void DocumentWidget::refreshDocumentList() {
    // 释放旧模型，避免内存泄漏
    QSqlQueryModel *oldModel = qobject_cast<QSqlQueryModel*>(docTable->model());
    delete oldModel;

    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;
    QString queryStr;

    // 根据权限动态生成查询（参数化查询）
    if (UserManager::instance().currentUser()->hasPermission("view_all_documents")) {
        queryStr = "SELECT doc_id AS '编号', title AS '标题', current_status AS '状态' FROM documents";
        query.prepare(queryStr);
    } else {
        queryStr = "SELECT doc_id AS '编号', title AS '标题', current_status AS '状态' "
                   "FROM documents WHERE applicant_id = ?";
        query.prepare(queryStr);
        query.addBindValue(UserManager::instance().currentUser()->id());
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "查询失败: " + query.lastError().text());
        return;
    }

    // 添加强制刷新逻辑
    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "错误", "模型刷新失败: " + model->lastError().text());
    }
    docTable->setModel(model);
    docTable->setColumnHidden(0, true); // 隐藏doc_id列
    docTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    docTable->setItemDelegateForColumn(2, new StatusColorDelegate(this));
}

void DocumentWidget::openWorkflowConfig() {
    // 实现流程配置对话框（示例）
    WorkflowConfigDialog dialog(this);
    dialog.exec();
}

void DocumentWidget::onApprovalProcessed(int docId) {
    refreshDocumentList();
    qDebug() << "批文" << docId << "审批完成";
}
