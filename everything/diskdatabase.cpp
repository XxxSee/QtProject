#include "diskdatabase.h"
#include <QDebug>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMutexLocker>
#include <QMapIterator>
#include "connectionpool.h"
#include "diskglobal.h"
#include "threadpool.h"

DiskDataBase::DiskDataBase(QObject *parent) :
    QObject(parent),
    m_dConnName("DiskDB"),
    m_dWhere(""),
    m_bInit(true)
{
    //ConnectionPool::openConnection(m_dConnName,"QSQLITE");
}

DiskDataBase::~DiskDataBase()
{
    ConnectionPool::clear();
}

DiskSqlite::DiskSqlite(QObject *parent) :
    DiskDataBase(parent)
{

}

DiskSqlite::~DiskSqlite()
{
    ConnectionPool::clear();
}

bool DiskSqlite::createTable()
{
    //连接数据库
    auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
    if (ret.first)
    {
        QSqlDatabase db = ret.second;
        QSqlQuery query(db);
        //清空表自增信息
        if (query.exec(QString("select count(*) from sqlite_master where type='table' and name='sqlite_sequence';"))) {
            if (query.next()) {
                //存在表则清空
                if (query.value(0).toInt() > 0) {
                    query.exec(QString("delete from sqlite_sequence;"));
                }
            }
        }
        //清空表
        if (query.exec(QString("select count(*) from sqlite_master where type='table' and name='filelist';"))) {
            if (query.next()) {
                //存在表则清空
                if (query.value(0).toInt() > 0) {
                    query.exec(QString("delete from filelist;"));
                }
                //不存在则创建
                else {
                    if (!query.exec(QString("create table filelist ( "
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "filename VARCHAR(256) NOT NULL, "
                            "suffix VARCHAR(64), "
                            "filepath VARCHAR(256), "
                            "filesize VARCHAR(32), "
                            "filetime VARCHAR(32), "
                            "diskname VARCHAR(32));"))) {
                        qDebug() << query.lastError().text();
                        return false;
                    }
                }
            }
        }
        else {
            qDebug() << query.lastError().text();
            return false;
        }
        m_bInit = false;
        return true;
    }
    return false;
}

bool DiskSqlite::insertTable()
{
    //连接数据库
    auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
    if (ret.first)
    {
        QSqlDatabase db = ret.second;
        QSqlQuery query(db);
        //qDebug() << "begin insert table";
        //显示开启事务
        query.exec(QString("begin;"));
        //开始插入数据
        int chunkCnt = 0;

        QMap<QString,QList<QFileInfo>>& files = DiskGlobal::files();
        query.prepare(QString("insert into filelist (filename, suffix, filepath, filesize, filetime, diskname) values (?,?,?,?,?,?);"));
        QVariantList filenames,suffixs,filepaths,filesizes,filetimes,disknames;
        for (auto it=files.begin(); it!=files.end() && ThreadPool::ins()->isRunning(); it++) {
            QList<QFileInfo> & infos = it.value();
            for (int i{0}; i<infos.size() && ThreadPool::ins()->isRunning(); i++) {
                //如果数据超过了100万，先提交一次，这里假设是10万提交一次影响效率
                if (chunkCnt != 0 && (chunkCnt % 999999 == 0)) {
                    query.addBindValue(filenames);
                    query.addBindValue(suffixs);
                    query.addBindValue(filepaths);
                    query.addBindValue(filesizes);
                    query.addBindValue(filetimes);
                    query.addBindValue(disknames);
                    if (!query.execBatch())
                        qDebug() << query.lastError().text();
                    query.exec(QString("commit"));
                    //重新开启事务
                    query.exec(QString("begin"));
                    query.prepare(QString("insert into filelist (filename, suffix, filepath, filesize, filetime, diskname) values (?,?,?,?,?,?);"));
                    filenames.clear();
                    suffixs.clear();
                    filepaths.clear();
                    filesizes.clear();
                    filetimes.clear();
                    chunkCnt = 0;
                }
                filenames << infos[i].fileName();
                suffixs << infos[i].suffix().toLower();
                filepaths << infos[i].absolutePath();
                filesizes << QString("%1").arg(infos[i].size());
                filetimes << infos[i].fileTime(QFile::FileModificationTime).toString("yyyy-MM-dd hh:mm:ss");
                disknames << it.key();
                chunkCnt++;
            }
        }
        query.addBindValue(filenames);
        query.addBindValue(suffixs);
        query.addBindValue(filepaths);
        query.addBindValue(filesizes);
        query.addBindValue(filetimes);
        query.addBindValue(disknames);
        if (chunkCnt > 0) {
            if (!query.execBatch()) {
                qDebug() << query.lastError().text();
            }
        }
        query.exec(QString("commit"));
        m_bInit = true;
        return true;
    }
    return false;
}

bool DiskSqlite::queryTable(const QString &cond)
{
    if (m_bInit) {
        m_dWhere = cond;
        if (cond.isEmpty())
            return false;
        auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
        if (!ret.first)
            return false;
        QString cond1 = QString("filename like '%%%1'").arg(cond);
        QString cond2 = QString("filepath like '%%%1'").arg(cond);
        QString cond3 = QString("(%1 or %2) order by filetime desc").arg(cond1).arg(cond2);
        ThreadPool::ins()->addTask(bind(&DiskSqlite::execQuery,this,cond,cond3));
        return true;
    }
    return false;
}

bool DiskSqlite::selectTable(const QString &)
{
    if (m_bInit) {
        auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
        if (ret.first) {
            QSqlDatabase db = ret.second;
            QSqlQuery query(db);
            QString queryStr = QString("select A.diskname, max(A.cnt) as max, min(A.cnt) as min from (select diskname, suffix, count(*) as cnt from filelist group by diskname, suffix) as A group by A.diskname");
            if (!query.exec(queryStr))
                return false;
            QVector<QString> disks;
            QVector<int> mins,maxs;
            m_mFinish.clear();
            while (query.next() && ThreadPool::ins()->isRunning()) {
                QString disk = query.value("diskname").toString();
                int min = query.value("min").toInt();
                int max = query.value("max").toInt();
                disks << disk;
                mins << min;
                maxs << max;
                m_mFinish[disk] = false;
            }
            for (int i{0}; i<disks.size(); i++) {
                QString disk = disks[i];
                int min = mins[i];
                int max = maxs[i];
                ThreadPool::ins()->addTask(bind(&DiskSqlite::execSelect,this,disk,min,max));
            }
        }
        return true;
    }
    return false;
}

void DiskSqlite::execQuery(const QString &condVal, const QString &condition)
{
    auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
    if (ret.first)
    {
        QSqlDatabase db = ret.second;
        QSqlQuery query(db);
        QString queryStr = QString("select * from filelist where 1=1 and %1").arg(condition);
        if (!query.exec(queryStr))
            return;
        while (query.next() && ThreadPool::ins()->isRunning()) {
            if (m_dWhere != condVal)
                break;
            FileInfo fileInfo;
            qint64 id = query.value("id").toUInt();
            fileInfo.name = query.value("filename").toString();
            fileInfo.suffix = query.value("suffix").toString();
            fileInfo.path = query.value("filepath").toString();
            fileInfo.size = query.value("filesize").toString();
            fileInfo.time = query.value("filetime").toString();
            fileInfo.disk = query.value("diskname").toString();
            emit DiskGlobal::ins()->sigQueryResult(m_dWhere, id, fileInfo);
        }
    }
    return;
}

void DiskSqlite::execSelect(const QString &disk, int min, int max)
{
    auto ret = ConnectionPool::openConnection(m_dConnName,"QSQLITE");
    if (ret.first)
    {
        QSqlDatabase db = ret.second;
        QSqlQuery query(db);
        int filter = (min+max)/100;
        QString execStr = QString("select suffix, count(*) as cnt from (select * from filelist where diskname = '%1') group by suffix having cnt >= %2").arg(disk).arg(filter);
        if (!query.exec(execStr))
            return ;
        while (query.next() && ThreadPool::ins()->isRunning()) {
            QString suffix = query.value("suffix").toString();
            if (suffix.isEmpty()) {
                suffix = QString("<null>");
            }
            qint64 cnt = query.value("cnt").toLongLong();
            emit DiskGlobal::ins()->sigSelectResult(disk,suffix,cnt);
        }
        if (filter > 0) {
            execStr = QString("select sum(B.cnt) as cnt from (select A.suffix, count(*) as cnt from (select * from filelist where diskname = '%1') as A group by A.suffix having cnt < %2) as B").arg(disk).arg(filter);
            if (!query.exec(execStr))
                return ;
            while (query.next() && ThreadPool::ins()->isRunning()) {
                qint64 cnt = query.value("cnt").toLongLong();
                emit DiskGlobal::ins()->sigSelectResult(disk,"other",cnt);
            }
        }
        updateFinish(disk,true);
    }
    return ;
}

void DiskSqlite::updateFinish(const QString &disk, bool finish)
{
    QMutexLocker locker(&m_dMtx);
    m_mFinish[disk] = finish;
    bool allFinish = true;
    QMapIterator<QString,bool> it(m_mFinish);
    while (it.hasNext()) {
        it.next();
        if (!it.value()) {
            allFinish = false;
            break;
        }
    }
    if (allFinish) {
        emit DiskGlobal::ins()->sigSelectResult("finish","",0);
    }
}
