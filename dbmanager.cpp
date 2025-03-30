#include "dbmanager.h"
#include <QMessageBox>

DBManager::DBManager(QObject *parent) : QObject(parent) {
    m_db = QSqlDatabase::addDatabase("QMYSQL", "doc_management_connection");
}

DBManager::~DBManager() {
    disconnect();
}

void DBManager::setConnectionParams(const QString &host, const QString &dbName, const QString &user, const QString &password) {
    m_host = host;
    m_dbName = dbName;
    m_user = user;
    m_password = password;
}

bool DBManager::connect() {
    // QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "doc_management_connection");
    m_db.setHostName(m_host);
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_user);
    m_db.setPassword(m_password);
    return m_db.open();
}

bool DBManager::connect(const QString &host,
                        const QString &dbName,
                        const QString &user,
                        const QString &password,
                        int port) {
    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        QMessageBox::critical(nullptr, "数据库错误",
                              "连接失败: " + m_db.lastError().text());
        return false;
    }
    return true;
}

void DBManager::disconnect() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

QSqlError DBManager::lastError() const {
    return m_db.lastError();
}
