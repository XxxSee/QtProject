#ifndef DISKGLOBAL_H
#define DISKGLOBAL_H

#include <QObject>
#include <QFileInfo>
#include <QMap>
#include <QScopedPointer>
#include "diskstruct.h"

class DiskGlobal : public QObject
{
    Q_OBJECT
public:
    ~DiskGlobal();
protected:
    DiskGlobal(QObject *parent = nullptr);
signals:
    void sigProgressInfo(const QString &info);
    void sigQueryResult(const QString &con, qint64 id, const FileInfo &file);
    void sigSelectResult(const QString &disk, const QString &suffix, qint64 count);
public:
    static DiskGlobal *ins();
    static void append(Qt::HANDLE threadId, const QString &root, QFileInfo &fileInfo);
    static void clear();
    static QMap<QString,QList<QFileInfo>> &files();
private:
    static QMap<Qt::HANDLE,QMap<QString,QList<QFileInfo>>> m_mThreadFiles;
    static QMap<QString,QList<QFileInfo>> m_mDiskFiles;
    static QScopedPointer<DiskGlobal> m_ins;
};

#endif // DISKGLOBAL_H
