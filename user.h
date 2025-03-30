#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

#include <QString>
#include <QJsonObject>

class User {
public:
    User(int id, const QString& username, int roleId, const QJsonObject& permissions)
        : m_id(id), m_username(username), m_roleId(roleId), m_permissions(permissions) {}

    bool hasPermission(const QString& permKey) const {
        if (m_permissions.isEmpty()) {
            qWarning() << "权限数据为空，用户ID:" << m_id;
            return false;
        }
        return m_permissions.value(permKey).toBool(false);
    }

    // Getters
    int id() const { return m_id; }
    QString username() const { return m_username; }
    int roleId() const { return m_roleId; }

private:
    int m_id;//user_id
    QString m_username;//username
    int m_roleId;//角色id
    QJsonObject m_permissions;//权限
};

class UserManager : public QObject {
    Q_OBJECT

public:
    static UserManager& instance() {
        static UserManager instance;
        return instance;
    }

private:
    UserManager(QObject *parent = nullptr) : QObject(parent) {} // 私有构造函数

public:

    bool login(const QString& username, const QString& password);

    // 用户登录验证
    bool authenticate(const QString& username, const QString& password);

    // 创建新用户（带自动加盐）
    bool createUser(const QString& username, const QString& password, int roleId);

    // 获取当前用户
    User* currentUser() const { return m_currentUser.get(); }

    QString generateSalt() const;

    QString hashPassword(const QString& password, const QString& salt) const;
    // User* currentUser() const { return m_currentUser.get(); }
signals:
    void userLoggedIn();
    void userLoggedOut();

private:

    std::unique_ptr<User> m_currentUser; // 改为unique_ptr
};
#endif // USER_H
