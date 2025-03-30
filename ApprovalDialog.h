#ifndef APPROVALDIALOG_H
#define APPROVALDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "DocumentManager.h"
#include "user.h"

class QPushButton;

class ApprovalDialog : public QDialog {
    Q_OBJECT
public:
    explicit ApprovalDialog(int docId, QWidget *parent = nullptr);

private slots:
    void onApproveClicked();
    void onRejectClicked();

private:
    int m_docId;
    QPushButton *btnApprove;
    QPushButton *btnReject;
};

#endif // APPROVALDIALOG_H
