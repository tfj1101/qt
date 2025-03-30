#include "ApprovalDialog.h"
#include "qpushbutton.h"
#include "qsqlerror.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

ApprovalDialog::ApprovalDialog(int docId, QWidget *parent)
    : QDialog(parent), m_docId(docId) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("审批操作", this);
    btnApprove = new QPushButton("通过", this);
    btnReject = new QPushButton("驳回", this);

    layout->addWidget(label);
    layout->addWidget(btnApprove);
    layout->addWidget(btnReject);

    connect(btnApprove, &QPushButton::clicked, this, &ApprovalDialog::onApproveClicked);
    connect(btnReject, &QPushButton::clicked, this, &ApprovalDialog::onRejectClicked);
}

void ApprovalDialog::onApproveClicked() {
    QSqlQuery query;
    query.prepare("UPDATE documents SET current_status = 'approved' WHERE doc_id = ?");
    query.addBindValue(m_docId);
    if (query.exec()) {
        DocumentManager::instance().logStatusChange(m_docId, "approved", UserManager::instance().currentUser()->id());
        QMessageBox::information(this, "成功", "批文已通过！");
        accept();
    } else {
        QMessageBox::critical(this, "错误", "操作失败: " + query.lastError().text());
    }
}

void ApprovalDialog::onRejectClicked() {
    QSqlQuery query;
    query.prepare("UPDATE documents SET current_status = 'rejected' WHERE doc_id = ?");
    query.addBindValue(m_docId);
    if (query.exec()) {
        DocumentManager::instance().logStatusChange(m_docId, "rejected", UserManager::instance().currentUser()->id());
        QMessageBox::information(this, "成功", "批文已驳回！");
        accept();
    } else {
        QMessageBox::critical(this, "错误", "操作失败: " + query.lastError().text());
    }
}
