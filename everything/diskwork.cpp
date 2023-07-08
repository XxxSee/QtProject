#include "diskwork.h"
#include <QDir>
#include <QFileInfo>
#include <QStorageInfo>
#include <QDateTime>
#include <QDebug>
#include "diskglobal.h"
#include "threadpool.h"
#include "diskdatabase.h"

DiskWork::DiskWork(QObject *parent) :
    QThread(parent),
    m_bDBInit(false)
{
    ThreadPool::ins()->start(10);
    m_pDB = new DiskSqlite(this);
    connect(this,&DiskWork::finished,this,[](){
        emit DiskGlobal::ins()->sigProgressInfo("finish");
    });
}

DiskWork::~DiskWork()
{
    ThreadPool::ins()->stop();
    quit();
    wait();
    //qDebug() << "~DiskWork";
}

void DiskWork::slotQueryTable(const QString &con)
{
    //if (m_bDBInit) {
        m_pDB->queryTable(con);
        //}
}

void DiskWork::slotSelectTable(const QString &exec)
{
    m_pDB->selectTable(exec);
}

void DiskWork::run()
{
    DiskGlobal::clear();
    QDateTime t1 = QDateTime::currentDateTime();
    traverseDisk();
    //qDebug() << "search file begin wait";
    ThreadPool::ins()->wait();
    //qDebug() << "search file end wait";
    //QDateTime t2 = QDateTime::currentDateTime();
    //qint64 consumet = t2.toSecsSinceEpoch() - t1.toSecsSinceEpoch();
    int size = 0;
    for (QList<QFileInfo> files : DiskGlobal::files()) {
        size += files.size();
    }
    //qDebug() << "search file consume time:" << consumet << "s" << " and find file count:" << size;
    //qDebug() << "begin insert database";
    t1 = QDateTime::currentDateTime();
    if (m_pDB->createTable()) {
        emit DiskGlobal::ins()->sigProgressInfo(QStringLiteral("正在插入数据库"));
        if (m_pDB->insertTable()) {
            m_bDBInit = true;
        }
    }
    //t2 = QDateTime::currentDateTime();
    //qDebug() << "insert database consume time:" << consumet << "s";
    DiskGlobal::clear();
}

void DiskWork::traverseDisk()
{
    QList<QStorageInfo> disks = QStorageInfo::mountedVolumes();
    for (int i{0}; i<disks.size()&&ThreadPool::ins()->isRunning(); i++) {
        disks[i].name();
        traverseDir(disks[i].rootPath().at(0), disks[i].rootPath(), 1);
    }
}

void DiskWork::traverseDir(const QString &root, const QString &path, int depth)
{
    QDir dir(path);
    if (!dir.exists())
        return;
    QFileInfoList files = dir.entryInfoList(QDir::AllDirs|QDir::Files|QDir::Hidden|QDir::NoDotAndDotDot);
    for (int i{0}; i<files.count()&&ThreadPool::ins()->isRunning();i++){
        if (files[i].isDir()) {
            if (depth > 2) {
                ThreadPool::ins()->addTask(bind(&DiskWork::depthFirst,this,root,files[i].filePath(),0));
            }
            else {
                emit DiskGlobal::ins()->sigProgressInfo(QStringLiteral("正在遍历%1").arg(files[i].filePath()));
                traverseDir(root,files[i].filePath(),depth+1);
            }
        }
        else {
            DiskGlobal::append(QThread::currentThreadId(),root,files[i]);
        }
    }
}

void DiskWork::depthFirst(const QString &root, const QString &path, int depth)
{
    QDir dir(path);
    if (!dir.exists())
        return;
    if (depth > 15) {
        breadthFirst(root,path);
    }
    emit DiskGlobal::ins()->sigProgressInfo(QStringLiteral("正在遍历%1").arg(path));
    QFileInfoList files = dir.entryInfoList(QDir::AllDirs|QDir::Files|QDir::Hidden|QDir::NoDotAndDotDot);
    for (int i{0}; i<files.count()&&ThreadPool::ins()->isRunning();i++){
        if (files[i].isDir()) {
            traverseDir(root,files[i].filePath(),depth+1);
        }
        else {
            DiskGlobal::append(QThread::currentThreadId(),root,files[i]);
        }
    }
}

void DiskWork::breadthFirst(const QString &root, const QString &path)
{
    QFileInfoList dirs;
    dirs << QFileInfo(path);
    for (int i{0}; i<dirs.size() && ThreadPool::ins()->isRunning(); i++) {
        if (!dirs[i].exists()) {
            continue;
        }
        QFileInfoList files = QDir(dirs[i].filePath()).entryInfoList(QDir::AllDirs|QDir::Hidden|QDir::NoDotAndDotDot);
        //QFileInfoList files = QDir(dirs[i].filePath()).entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
        for (int j{0}; j<files.size() && ThreadPool::ins()->isRunning(); j++) {
            if (files[i].isDir()) {
                dirs << files[i];
            }
            else {
                DiskGlobal::append(QThread::currentThreadId(),root,files[i]);
            }
        }
    }
}
