#include "connectionpool.h"

#include <QDebug>
#include <QtSql>
#include <QString>
#include <QThread>
#include <QCoreApplication>

QList<QString> ConnectionPool::s_lConnNames = QList<QString>();

// 获取数据库连接
QPair<bool, QSqlDatabase> ConnectionPool::openConnection(const QString &connectionName, const QString &Driver) {
    // 1. 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    // 2. 如果连接已经存在，复用它，而不是重新创建
    //    2.1 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
    // 3. 如果连接不存在，则创建连接
    // 4. 线程结束时，释放在此线程中创建的数据库连接

    // [1] 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    QString baseConnectionName = "conn_" + QString::number(quint64(QThread::currentThread()), 16);
    QString fullConnectionName = baseConnectionName + connectionName;

    if (QSqlDatabase::contains(fullConnectionName)) {
        // [2] 如果连接已经存在，复用它，而不是重新创建
        QSqlDatabase existingDb = QSqlDatabase::database(fullConnectionName);

        // [2.1] 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
        QSqlQuery query("SELECT 1", existingDb);

        if (query.lastError().type() != QSqlError::NoError && !existingDb.open()) {
            qDebug().noquote() << "Open datatabase error:" << existingDb.lastError().text();
            return qMakePair(false, QSqlDatabase());
        }

        return qMakePair(true, existingDb);
    } else {
        // [3] 如果连接不存在，则创建连接
        if (qApp != nullptr) {
            // [4] 线程结束时，释放在此线程中创建的数据库连接
            QObject::connect(QThread::currentThread(), &QThread::finished, qApp, [fullConnectionName] {
                if (QSqlDatabase::contains(fullConnectionName)) {
                    QSqlDatabase::removeDatabase(fullConnectionName);
                    if (s_lConnNames.contains(fullConnectionName))
                        s_lConnNames.removeOne(fullConnectionName);
                    qDebug().noquote() << QString("Connection deleted: %1").arg(fullConnectionName);
                }
            });
        }
        return createConnection(fullConnectionName,Driver);
    }
}

void ConnectionPool::clear()
{
    for (int i{0}; i<s_lConnNames.size(); i++) {
        if (QSqlDatabase::contains(s_lConnNames[i])) {
            QSqlDatabase::removeDatabase(s_lConnNames[i]);
            qDebug().noquote() << QString("Connection clear: %1").arg(s_lConnNames[i]);
        }
    }
    s_lConnNames.clear();
}

// 创建数据库连接
QPair<bool, QSqlDatabase> ConnectionPool::createConnection(const QString &connectionName, const QString &Driver) {
    static int sn = 0;

    // 创建一个新的数据库连接
    QSqlDatabase db;
    if (Driver == "QMYSQL") {
        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("filelist");
        db.setUserName("root");
        db.setPassword("123456");
    }
    else if (Driver == "QSQLITE") {
        db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        db.setDatabaseName("sysfiles.db");
    }
    if (db.open()) {
        s_lConnNames.append(connectionName);
        qDebug().noquote() << QString("Connection created: %1, sn: %2").arg(connectionName).arg(++sn);
        return qMakePair(true, db);
    } else {
        qDebug().noquote() << "Create connection error:" << db.lastError().text();
        return qMakePair(false, QSqlDatabase());
    }
}
