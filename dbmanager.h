#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class DBManager : public QObject {
    Q_OBJECT

private:
    DBManager(QObject *parent = nullptr); // 私有构造函数


public:

    ~DBManager();

    // 禁止拷贝和赋值
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    bool connect(const QString &host,
                 const QString &dbName,
                 const QString &user,
                 const QString &password,
                 int port = 3306);

    bool connect();

    void disconnect();
    QSqlError lastError() const;

    // 设置数据库连接参数
    void setConnectionParams(const QString &host, const QString &dbName, const QString &user, const QString &password);

public:
    static DBManager& instance() {
        static DBManager instance;
        return instance;
    }

private:
    QSqlDatabase m_db;

    QString m_host= "localhost", m_dbName= "piwen", m_user = "root", m_password= "root";

};

#endif // DBMANAGER_H
