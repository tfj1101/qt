#include "DocumentDetailsDialog.h"
#include "DocumentManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

DocumentDetailsDialog::DocumentDetailsDialog(int docId, QWidget *parent)
    : QDialog(parent), m_docId(docId) {
    // 初始化界面
    QVBoxLayout *layout = new QVBoxLayout(this);

    titleLabel = new QLabel("标题:", this);
    contentEdit = new QTextEdit(this);
    contentEdit->setReadOnly(true);
    statusLabel = new QLabel("状态:", this);

    layout->addWidget(titleLabel);
    layout->addWidget(contentEdit);
    layout->addWidget(statusLabel);

    // 加载数据
    loadDocumentDetails(docId);
}

void DocumentDetailsDialog::loadDocumentDetails(int docId) {
    QJsonObject details = DocumentManager::instance().getDocumentDetails(docId);
    titleLabel->setText("标题: " + details["title"].toString());
    contentEdit->setText(details["content"].toString());
    statusLabel->setText("状态: " + details["status"].toString());
}
