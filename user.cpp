#include "user.h"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QJsonDocument>
#include<QSqlError>
#include<memory>



bool UserManager::createUser(const QString& username,
                             const QString& password,
                             int roleId) {
    QSqlQuery query;
    QString salt = generateSalt();
    QString hashedPw = hashPassword(password, salt);

    query.prepare("INSERT INTO users (username, password_hash, salt, role_id) "
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashedPw);
    query.addBindValue(salt);
    query.addBindValue(roleId);
    if (!query.exec()) {
        qCritical() << "创建用户失败:" << query.lastError().text();
        return false;
    }
    return true;

}


QString UserManager::hashPassword(const QString& password, const QString& salt) const {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    QByteArray combined = (password + salt).toUtf8(); // 密码 + 盐值
    hash.addData(combined); // 添加数据到哈希对象
    return QString(hash.result().toHex()); // 返回十六进制哈希结果
}

bool UserManager::login(const QString& username, const QString& password) {
    QSqlQuery query(QSqlDatabase::database("doc_management_connection"));
    query.prepare("SELECT u.user_id, u.password_hash, u.salt, u.role_id, r.permissions "
                  "FROM users u JOIN roles r ON u.role_id = r.role_id "
                  "WHERE u.username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "SQL Error:" << query.lastError().text();
        return false;
    }

    if (!query.next()) return false;

    // 调试输出
    int roleId = query.value("role_id").toInt();
    qDebug() << "Database role_id:" << roleId;

    QString storedHash = query.value("password_hash").toString();
    QString salt = query.value("salt").toString();
    QString inputHash = hashPassword(password, salt);

    if (inputHash == storedHash) {
        QJsonObject permissions = QJsonDocument::fromJson(
                                      query.value("permissions").toString().toUtf8()
                                      ).object();

        m_currentUser = std::make_unique<User>(
            query.value("user_id").toInt(),
            username,
            roleId, // 直接使用已获取的 roleId
            permissions
            );
        return true;
    }
    return false;
}


//登录验证
bool UserManager::authenticate(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT user_id, password_hash, salt, role_id, permissions "
                  "FROM users JOIN roles ON users.role_id = roles.role_id "
                  "WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
        qCritical() << "数据库查询失败:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qWarning() << "用户不存在:" << username;
        return false;
    }

    QString storedHash = query.value("password_hash").toString();
    QString salt = query.value("salt").toString();
    QString inputHash = hashPassword(password, salt);

    if (inputHash != storedHash) {
        qWarning() << "密码验证失败:" << username;
        return false;
    }

    QByteArray permissionsJson = query.value("permissions").toByteArray();
    QJsonDocument doc = QJsonDocument::fromJson(permissionsJson);
    if (doc.isNull()) {
        qCritical() << "权限数据格式无效:" << permissionsJson;
        return false;
    }

    m_currentUser = std::make_unique<User>(
        query.value("user_id").toInt(),
        username,
        query.value("role_id").toInt(),
        doc.object()
        );

    emit userLoggedIn();
    return true;
}
