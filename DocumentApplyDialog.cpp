#include "ApprovalWorkflow.h"

#include"DocumentApplyDialog.h"
#include"user.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>



#include <QDialogButtonBox>
#include <QFileDialog>

DocumentApplyDialog::DocumentApplyDialog(QWidget *parent)
    : QDialog(parent)
{
    QFormLayout *form = new QFormLayout(this);

    // 标题输入框
    titleEdit = new QLineEdit(this);
    form->addRow("标题:", titleEdit);

    // 内容输入框
    contentEdit = new QTextEdit(this);
    form->addRow("内容:", contentEdit);

    // 上传按钮
    btnUpload = new QPushButton("上传附件", this);
    connect(btnUpload, &QPushButton::clicked, this, &DocumentApplyDialog::onUploadClicked);
    form->addRow("附件:", btnUpload);

    // 确定/取消按钮
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

// 其他成员函数实现...
QString DocumentApplyDialog::getTitle() const {
    return titleEdit->text();
}

QString DocumentApplyDialog::getContent() const {
    return contentEdit->toPlainText();
}

QString DocumentApplyDialog::getAttachmentPath() const {
    return attachmentPath;
}

// void DocumentApplyDialog::onUploadClicked() {
//     QString path = QFileDialog::getOpenFileName(this, "选择附件");
//     if (!path.isEmpty()) {
//         attachmentPath = path;
//     }
// }


void DocumentApplyDialog::onUploadClicked() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "选择附件",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "所有文件 (*.*)"
        );

    if (!path.isEmpty()) {
        // 验证文件大小(例如限制10MB)
        QFileInfo fileInfo(path);
        if (fileInfo.size() > 10 * 1024 * 1024) {
            QMessageBox::warning(this, "错误", "附件大小不能超过10MB");
            return;
        }
        attachmentPath = path;
        btnUpload->setText(fileInfo.fileName()); // 显示文件名
    }
}
