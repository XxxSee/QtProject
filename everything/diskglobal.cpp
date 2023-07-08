#include "diskglobal.h"
#include <QMutex>

QMap<Qt::HANDLE,QMap<QString,QList<QFileInfo>>> DiskGlobal::m_mThreadFiles;
QMap<QString,QList<QFileInfo>> DiskGlobal::m_mDiskFiles;
QScopedPointer<DiskGlobal> DiskGlobal::m_ins;
QMutex diskglobalmtx;

DiskGlobal::DiskGlobal(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<FileInfo>("FileInfo");
    qRegisterMetaType<FileInfo>("FileInfo&");
}

DiskGlobal *DiskGlobal::ins()
{
    if (!m_ins.data()) {
        QMutexLocker lock(&diskglobalmtx);
        if (!m_ins.data()) {
            m_ins.reset(new DiskGlobal);
        }
    }
    return m_ins.data();
}

DiskGlobal::~DiskGlobal()
{

}

void DiskGlobal::append(Qt::HANDLE threadId, const QString &root, QFileInfo &fileInfo)
{
    m_mThreadFiles[threadId][root] << fileInfo;
}

void DiskGlobal::clear()
{
    m_mThreadFiles.clear();
    m_mDiskFiles.clear();
}

QMap<QString, QList<QFileInfo>> &DiskGlobal::files()
{
    if (m_mDiskFiles.isEmpty()) {
        for (auto ts : m_mThreadFiles) {
            for (auto it = ts.begin(); it != ts.end(); it++) {
                m_mDiskFiles[it.key()] << it.value();
            }
        }
    }
    return m_mDiskFiles;
}
