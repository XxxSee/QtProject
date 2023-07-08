#ifndef DISKDATABASE_H
#define DISKDATABASE_H

#include <QObject>
#include <QMap>
#include <QMutex>
class DiskDataBase : public QObject
{
    Q_OBJECT
public:
    explicit DiskDataBase(QObject *parent = nullptr);
    virtual ~DiskDataBase();
    virtual bool createTable() = 0;
    virtual bool insertTable() = 0;
    virtual bool queryTable(const QString &cond) = 0;
    virtual bool selectTable(const QString &exec) = 0;
protected:
    QString m_dConnName;
    QString m_dWhere;
    bool m_bInit;
};

class DiskSqlite : public DiskDataBase
{
public:
    explicit DiskSqlite(QObject *parent = nullptr);
    ~DiskSqlite();
    bool createTable() override;
    bool insertTable() override;
    bool queryTable(const QString &cond) override;
    bool selectTable(const QString &exec) override;
private:
    void execQuery(const QString &condVal, const QString &condition);
    void execSelect(const QString &disk, int min, int max);
    void updateFinish(const QString &disk, bool finish);
private:
    QMap<QString,bool> m_mFinish;
    QMutex m_dMtx;
};

#endif // DISKDATABASE_H
