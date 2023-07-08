#include "connectionpool.h"

#include <QDebug>
#include <QtSql>
#include <QString>
#include <QThread>
#include <QCoreApplication>

QList<QString> ConnectionPool::s_lConnNames = QList<QString>();

// ��ȡ���ݿ�����
QPair<bool, QSqlDatabase> ConnectionPool::openConnection(const QString &connectionName, const QString &Driver) {
    // 1. �������ӵ�ȫ��: �����̵߳ĵ�ַ�ʹ�������� connectionName����Ϊͬһ���߳̿������봴��������ݿ�����
    // 2. ��������Ѿ����ڣ������������������´���
    //    2.1 ��������ǰ�������ݿ⣬������ӶϿ����������½������� (����: �ر����ݿ⼸���Ӻ����������ٴη������ݿ�)
    // 3. ������Ӳ����ڣ��򴴽�����
    // 4. �߳̽���ʱ���ͷ��ڴ��߳��д��������ݿ�����

    // [1] �������ӵ�ȫ��: �����̵߳ĵ�ַ�ʹ�������� connectionName����Ϊͬһ���߳̿������봴��������ݿ�����
    QString baseConnectionName = "conn_" + QString::number(quint64(QThread::currentThread()), 16);
    QString fullConnectionName = baseConnectionName + connectionName;

    if (QSqlDatabase::contains(fullConnectionName)) {
        // [2] ��������Ѿ����ڣ������������������´���
        QSqlDatabase existingDb = QSqlDatabase::database(fullConnectionName);

        // [2.1] ��������ǰ�������ݿ⣬������ӶϿ����������½������� (����: �ر����ݿ⼸���Ӻ����������ٴη������ݿ�)
        QSqlQuery query("SELECT 1", existingDb);

        if (query.lastError().type() != QSqlError::NoError && !existingDb.open()) {
            qDebug().noquote() << "Open datatabase error:" << existingDb.lastError().text();
            return qMakePair(false, QSqlDatabase());
        }

        return qMakePair(true, existingDb);
    } else {
        // [3] ������Ӳ����ڣ��򴴽�����
        if (qApp != nullptr) {
            // [4] �߳̽���ʱ���ͷ��ڴ��߳��д��������ݿ�����
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

// �������ݿ�����
QPair<bool, QSqlDatabase> ConnectionPool::createConnection(const QString &connectionName, const QString &Driver) {
    static int sn = 0;

    // ����һ���µ����ݿ�����
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
