#ifndef DOCUMENTAPPLYDIALOG_H
#define DOCUMENTAPPLYDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <QDialogButtonBox>

//批文申请对话框
class DocumentApplyDialog : public QDialog {
    Q_OBJECT
public:
    explicit DocumentApplyDialog(QWidget *parent = nullptr);
    QString getTitle() const;
    QString getContent() const;
    QString getAttachmentPath() const;
    void onUploadClicked();

private:
    QLineEdit *titleEdit;
    QTextEdit *contentEdit;
    QPushButton *btnUpload;
    QString attachmentPath;
};



#endif // DOCUMENTAPPLYDIALOG_H
