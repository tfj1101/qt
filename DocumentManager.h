#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonObject>
#include <QTableView>

//批文管理者
class DocumentManager : public QObject {
    Q_OBJECT

public:
    // 构造函数
    explicit DocumentManager(QObject *parent = nullptr);

    // 单例访问
    static DocumentManager& instance();

    // 提交批文
       bool submitDocument(const QString &title, const QString &content, int applicantId, const QString& attachmentPath = QString()); // 添加第四个参数

    // 加载状态历史到表格视图
    void loadStatusHistory(int docId, QTableView *view);

    // 获取批文详细信息
    QJsonObject getDocumentDetails(int docId);

    // 记录状态变更历史
    void logStatusChange(int docId, const QString &status, int userId);

    // // 添加导入Excel方法声明
    // bool importFromExcel(const QString &filePath);

    QSqlError lastError() const;

signals:
    // 批文提交成功信号
    void documentSubmitted(int docId);

private:


    // 数据库连接
    QSqlDatabase db;
};

#endif // DOCUMENTMANAGER_H
