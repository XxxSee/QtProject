#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H


#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QPair>
class ConnectionPool {
public:
    /**
     * @brief 获取数据库连接，连接使用完后不需要手动关闭，数据库连接池会在使用此连接的线程结束后自动关闭连接。
     * 传入的连接名 connectionName 默认为空 (内部会为连接名基于线程的信息创建一个唯一的前缀)，
     * 如果同一个线程需要使用多个不同的数据库连接，可以传入不同的 connectionName
     *
     * @param connectionName 连接的名字
     * @return 返回数据库连接
     */
    static QPair<bool, QSqlDatabase> openConnection(const QString &connectionName = QString(), const QString &Driver = "QMYSQL");
    static void clear();
private:
    static QPair<bool, QSqlDatabase> createConnection(const QString &connectionName, const QString &Driver = "QMYSQL"); // 创建数据库连接
    static QList<QString> s_lConnNames;
};

#endif // CONNECTIONPOOL_H
