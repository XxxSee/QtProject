#ifndef DISKWORK_H
#define DISKWORK_H

#include <QThread>

class DiskDataBase;
class DiskWork : public QThread
{
    Q_OBJECT
public:
    DiskWork(QObject *parent = nullptr);
    ~DiskWork();
public slots:
    void slotQueryTable(const QString &con);
    void slotSelectTable(const QString &exec);
protected:
    void run() override;
private:
    void traverseDisk();
    void traverseDir(const QString &root, const QString &path, int depth);
    void depthFirst(const QString &root, const QString &path, int depth);
    void breadthFirst(const QString &root, const QString &path);
private:
    DiskDataBase *m_pDB;
    bool m_bDBInit;
};

#endif // DISKWORK_H
