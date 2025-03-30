#include "ApprovalWorkflow.h"
#include "User.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

// 单例实例初始化
ApprovalWorkflow& ApprovalWorkflow::instance() {
    static ApprovalWorkflow instance;
    return instance;
}

// 私有构造函数
ApprovalWorkflow::ApprovalWorkflow(QObject *parent)
    : QObject(parent)
{
    // 初始化数据库连接（使用与主程序相同的命名连接）
    m_db = QSqlDatabase::database("doc_management_connection");
}

// 处理审批操作（支持多级动态审批）
void ApprovalWorkflow::processApproval(int docId, int approverId, ApprovalStep step, bool isApproved) {
    if (!m_db.isOpen()) {
        emit approvalFailed("数据库未连接");
        return;
    }

    m_db.transaction(); // 开启事务

    try {
        // 1. 插入审批记录
        QSqlQuery query(m_db);
        query.prepare(
            "INSERT INTO approvals (doc_id, approver_id, approval_step, decision) "
            "VALUES (?, ?, ?, ?)"
            );
        query.addBindValue(docId);
        query.addBindValue(approverId);
        query.addBindValue(static_cast<int>(step));
        query.addBindValue(isApproved ? "approved" : "rejected");

        if (!query.exec()) {
            throw query.lastError();
        }

        // 2. 更新文档状态
        updateDocumentStatus(docId, isApproved);

        m_db.commit(); // 提交事务
        emit approvalProcessed(docId);
    } catch (const QSqlError &e) {
        m_db.rollback(); // 回滚事务
        qCritical() << "审批流程错误:" << e.text();
        emit approvalFailed("审批操作失败: " + e.text());
    }
}

// 更新文档状态（支持动态审批层级）
void ApprovalWorkflow::updateDocumentStatus(int docId, bool isApproved) {
    QSqlQuery query(m_db);
    QString newStatus;

    if (isApproved) {
        // 获取当前已通过的审批步骤数
        query.prepare(
            "SELECT COUNT(*) FROM approvals "
            "WHERE doc_id = ? AND decision = 'approved'"
            );
        query.addBindValue(docId);

        if (query.exec() && query.next()) {
            int approvedSteps = query.value(0).toInt();

            // 获取该文档类型所需的总审批步骤（需扩展业务逻辑）
            int requiredSteps = 2; // 示例：默认需要2级审批

            newStatus = (approvedSteps >= requiredSteps) ? "approved" : "pending";
        }
    } else {
        newStatus = "rejected";
    }

    // 更新文档主状态
    query.prepare("UPDATE documents SET current_status = ? WHERE doc_id = ?");
    query.addBindValue(newStatus);
    query.addBindValue(docId);
    if (!query.exec()) {
        throw query.lastError();
    }

    // 记录状态变更历史
    query.prepare(
        "INSERT INTO document_status (doc_id, status, changed_by) "
        "VALUES (?, ?, ?)"
        );
    query.addBindValue(docId);
    query.addBindValue(newStatus);
    query.addBindValue(UserManager::instance().currentUser()->id());

    if (!query.exec()) {
        throw query.lastError();
    }
}
