#include "DocumentManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include<QHeaderView>
#include<QSqlQueryModel>
DocumentManager::DocumentManager(QObject *parent) : QObject(parent) {
    // 获取数据库连接
    db = QSqlDatabase::database("doc_management_connection");
}

// bool DocumentManager::importFromExcel(const QString &filePath) {
//     QXlsx::Document xlsx(filePath);
//     if (!xlsx.load()) {
//         qWarning() << "无法打开Excel文件:" << filePath;
//         return false;
//     }

//     db.transaction();
//     try {
//         QSqlQuery query(db);
//         query.prepare("INSERT INTO documents (title, content, applicant_id) VALUES (?, ?, ?)");

//         int rowCount = xlsx.dimension().lastRow();
//         for (int row = 2; row <= rowCount; ++row) { // 跳过标题行
//             QString title = xlsx.cellAt(row, 1)->value().toString();
//             QString content = xlsx.cellAt(row, 2)->value().toString();

//             query.addBindValue(title);
//             query.addBindValue(content);
//             query.addBindValue(1); // 假设申请人ID为1
//             if (!query.exec()) {
//                 throw query.lastError();
//             }
//         }

//         db.commit();
//         return true;
//     } catch (const QSqlError &e) {
//         db.rollback();
//         qCritical() << "导入失败:" << e.text();
//         return false;
//     }
// }


// 提交批文到数据库
bool DocumentManager::submitDocument(
    const QString &title,
    const QString &content,
    int applicantId,
    const QString &attachmentPath // 正确添加参数
    ) {
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "错误", "数据库未连接");
        return false;
    }



    db.transaction(); // 开启事务

    try {
        // 1. 插入批文记录
        QSqlQuery query(db);

        query.prepare(
            "INSERT INTO documents (title, content, applicant_id, current_status, attachment_path) "
            "VALUES (:title, :content, :applicantId, 'pending', :attachmentPath)"
            );
        query.bindValue(":attachmentPath", attachmentPath);
        query.bindValue(":title", title);
        query.bindValue(":content", content);
        query.bindValue(":applicantId", applicantId);

        if (!query.exec()) {
            throw query.lastError();
        }

        // 2. 记录初始状态变更
        int docId = query.lastInsertId().toInt();
        logStatusChange(docId, "pending", applicantId);

        db.commit(); // 提交事务
        return true;
        emit documentSubmitted(docId); // 发送提交成功信号

    } catch (const QSqlError &e) {
        db.rollback(); // 回滚事务
        return false;
        QMessageBox::critical(nullptr, "错误", "提交失败: " + e.text());
        qCritical() << "Submit Document Error:" << e.text();
    }
}

// 记录状态变更历史
void DocumentManager::logStatusChange(int docId, const QString &status, int userId) {
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO document_status (doc_id, status, changed_by) "
        "VALUES (:docId, :status, :userId)"
        );
    query.bindValue(":docId", docId);
    query.bindValue(":status", status);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qCritical() << "Failed to log status:" << query.lastError().text();
        throw query.lastError();
    }
}

// 加载状态历史到表格视图
void DocumentManager::loadStatusHistory(int docId, QTableView *view) {
    QSqlQueryModel *model = new QSqlQueryModel(view);
    model->setQuery(
        QString(
            "SELECT "
            "  status AS '状态', "
            "  u.username AS '操作人', "
            "  changed_at AS '时间' "
            "FROM document_status ds "
            "JOIN users u ON ds.changed_by = u.user_id "
            "WHERE doc_id = %1 "
            "ORDER BY changed_at DESC"
            ).arg(docId),
        db
        );

    // 设置表格属性
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setSelectionMode(QAbstractItemView::NoSelection);

    // 检查查询错误
    if (model->lastError().isValid()) {
        QMessageBox::warning(nullptr, "警告", "无法加载状态历史: " + model->lastError().text());
    }
}

// 获取批文详细信息
QJsonObject DocumentManager::getDocumentDetails(int docId) {
    QJsonObject details;
    QSqlQuery query(db);
    query.prepare(
        "SELECT "
        "  title, content, attachment_path, "
        "  u.username AS applicant, "
        "  current_status "
        "FROM documents d "
        "JOIN users u ON d.applicant_id = u.user_id "
        "WHERE doc_id = ?"
        );
    query.addBindValue(docId);

    if (query.exec() && query.next()) {
        details["title"] = query.value("title").toString();
        details["content"] = query.value("content").toString();
        details["applicant"] = query.value("applicant").toString();
        details["status"] = query.value("current_status").toString();
        details["attachment"] = query.value("attachment_path").toString();
    } else {
        qWarning() << "Failed to get document details:" << query.lastError().text();
    }

    return details;
}

// 静态实例化方法（单例模式）
DocumentManager& DocumentManager::instance() {
    static DocumentManager instance;
    return instance;
}


QSqlError DocumentManager::lastError() const {
    return db.lastError();
}
