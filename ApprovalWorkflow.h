#ifndef APPROVALWORKFLOW_H
#define APPROVALWORKFLOW_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

class ApprovalWorkflow : public QObject {
    Q_OBJECT

public:
    // 审批步骤枚举（初审、复审等）
    enum ApprovalStep {
        InitialReview,  // 初审
        FinalReview     // 终审
    };
    Q_ENUM(ApprovalStep)  // 启用枚举元数据支持

    // 单例访问方法
    static ApprovalWorkflow& instance();




    // 处理审批操作
    void processApproval(int docId, int approverId, ApprovalStep step, bool isApproved);


signals:
    // 审批完成信号（参数：批文ID）
    void approvalProcessed(int docId);

    // 审批失败信号（参数：错误信息）
    void approvalFailed(const QString &error);

private:
    // 更新批文状态（私有工具方法）
    void updateDocumentStatus(int docId, bool isApproved);
        // 构造函数
    explicit ApprovalWorkflow(QObject *parent = nullptr);

    // 数据库连接对象
    QSqlDatabase m_db;
};

#endif // APPROVALWORKFLOW_H
