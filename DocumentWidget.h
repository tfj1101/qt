#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>  // 添加布局头文件
#include <QSqlError>

class DocumentWidget : public QWidget {
    Q_OBJECT
public:
    explicit DocumentWidget(QWidget *parent = nullptr);

    QString lastError() const;

private slots:
    //申请批文
    void onApplyDocument();
    //同意批文
    void onApproveDocument();

    //更新批文列表
    void refreshDocumentList();

    //检验权限
    void checkPermissions();


public slots:
    void onApprovalProcessed(int docId);


private:
    QHBoxLayout *btnLayout = nullptr;
    QPushButton *btnConfig = nullptr;
    QTableView *docTable = nullptr;    //
    QPushButton *btnApply = nullptr;   //
    QPushButton *btnApprove = nullptr;

    QString m_lastError;  // 添加错误信息存储成员

    void openWorkflowConfig();
};
#endif
