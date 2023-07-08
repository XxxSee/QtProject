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
     * @brief ��ȡ���ݿ����ӣ�����ʹ�������Ҫ�ֶ��رգ����ݿ����ӳػ���ʹ�ô����ӵ��߳̽������Զ��ر����ӡ�
     * ����������� connectionName Ĭ��Ϊ�� (�ڲ���Ϊ�����������̵߳���Ϣ����һ��Ψһ��ǰ׺)��
     * ���ͬһ���߳���Ҫʹ�ö����ͬ�����ݿ����ӣ����Դ��벻ͬ�� connectionName
     *
     * @param connectionName ���ӵ�����
     * @return �������ݿ�����
     */
    static QPair<bool, QSqlDatabase> openConnection(const QString &connectionName = QString(), const QString &Driver = "QMYSQL");
    static void clear();
private:
    static QPair<bool, QSqlDatabase> createConnection(const QString &connectionName, const QString &Driver = "QMYSQL"); // �������ݿ�����
    static QList<QString> s_lConnNames;
};

#endif // CONNECTIONPOOL_H
