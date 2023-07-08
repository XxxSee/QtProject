#include "download.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFileInfo>
#include <QDebug>
#include <QThread>
#include <QtMath>
#include <QSettings>
#include <QApplication>

Download::Download(int index, QObject *parent) :
    QThread(parent),
    m_dIndex(index)
{

}

Download::~Download()
{
    quit();
    wait();
}

void Download::startDownload(const QUrl &url, uint start, uint end, uint hasDown)
{
    m_dUrl = url;
    m_dStart = start;
    m_dEnd = end;
    m_dHasDownSize = 0;
    m_dLastDownSize = 0;
    m_dStartOffet = hasDown;
    qDebug() << m_dIndex << m_dStart << m_dEnd;
    this->start();
}

void Download::run()
{
    QEventLoop loop;
    QNetworkAccessManager am;
    QObject obj;
    QNetworkReply *reply = nullptr;
    QNetworkRequest request;
    request.setUrl(m_dUrl);
    QString range = QString("bytes=%1-%2").arg(m_dStart+m_dStartOffet).arg(m_dEnd);
    request.setRawHeader("Range", range.toLatin1());
    reply = am.get(request);
    connect(reply,&QNetworkReply::readyRead,&obj,[&](){
        QByteArray buffer = reply->readAll();
        emit sigWrite(buffer, m_dStart + m_dStartOffet + m_dHasDownSize);
        m_dLastDownSize = m_dHasDownSize;
        m_dHasDownSize += buffer.size();
    });
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    delete reply;
}

DownloadManager::DownloadManager(const QUrl &url, const QString &name, uint size, int num, const QString &uid, bool resume, QObject *parent) :
    QObject(parent),
    m_dUrl(url),
    m_dName(name),
    m_dUuid(uid),
    m_dSize(size),
    m_dNum(num),
    m_dHasDownSize(0),
    m_pFile(new QFile(name,this)),
    m_bResume(resume),
    m_bFinish(true)
{

}

DownloadManager::~DownloadManager()
{
    if (!m_bFinish)
        saveResumeInfo();
}

bool DownloadManager::start()
{
    if (!m_bResume) {
        return startNew();
    }
    else {
        return startResume();
    }
}

void DownloadManager::pause(bool b)
{
    if (b) {
        int hasDownSum = 0;
        for (auto it=m_mLoaders.begin(); it!=m_mLoaders.end(); it++) {
            it.value()->quit();
            it.value()->wait();
            hasDownSum += it.value()->m_dLastDownSize;
        }
        m_dHasDownSize = hasDownSum;
    }
    else {
        for (auto it=m_mLoaders.begin(); it!=m_mLoaders.end(); it++) {
            it.value()->startDownload(it.value()->m_dUrl,it.value()->m_dStart,it.value()->m_dEnd,it.value()->m_dLastDownSize);
        }
    }
}

void DownloadManager::remove()
{
    for (auto it=m_mLoaders.begin(); it!=m_mLoaders.end(); it++) {
        it.value()->quit();
        it.value()->wait();
    }
    m_pFile->close();
    m_bFinish = true;
    QString path = QApplication::applicationDirPath()+"/resume.ini";
    QSettings sets(path,QSettings::IniFormat);
    sets.remove(m_dUuid);
    sets.remove(m_dUuid+"-thread");
    int size = sets.beginReadArray("FileID");
    bool flag = false;
    for (int i{0}; i<size; i++) {
        sets.setArrayIndex(i);
        QString id = sets.value("id").toString();
        if (id == m_dUuid) {
            sets.remove("id");
            flag = true;
            break;
        }
    }
    sets.endArray();
    if (flag) {
        sets.setValue("FileID/size",sets.value("FileID/size").toUInt()-1);
    }
    sets.sync();
}

bool DownloadManager::startNew()
{
    if (!m_pFile->open(QIODevice::WriteOnly)) {
        return false;
    }
    emit value(0);
    m_bFinish = false;
    m_dHasDownSize = 0;
    int perSize = qCeil(m_dSize*1./m_dNum);
    for (int i{0}; i<m_dNum; i++) {
        qint64 start = perSize*i;
        qint64 end = perSize*(i+1) - 1;
        if (i == (m_dNum-1)) {
            end = start + m_dSize - perSize*i;
        }
        Download *loader = new Download(i,this);
        m_mLoaders[i] = loader;
        connect(loader,&Download::sigWrite,this,&DownloadManager::slotWrite);
        loader->startDownload(m_dUrl,start,end);
    }
    return true;
}

bool DownloadManager::startResume()
{
    if (!m_pFile->open(QIODevice::ReadWrite)) {
        return false;
    }
    m_bFinish = false;
    QString path = QApplication::applicationDirPath()+"/resume.ini";
    QSettings sets(path,QSettings::IniFormat);
    sets.beginGroup(m_dUuid);
    m_dHasDownSize = sets.value("hasDownSize").toUInt();
    emit value(m_dHasDownSize/1000/1000);
    sets.endGroup();
    QMap<int,uint> mHasDownSize;
    int size = sets.beginReadArray(m_dUuid+"-thread");
    for (int i{0}; i<size; i++) {
        sets.setArrayIndex(i);
        uint index = sets.value("index").toUInt();
        uint hasDownSize = sets.value("hasDownSize").toUInt();
        mHasDownSize[index] = hasDownSize;
    }
    sets.endArray();

    int perSize = qCeil(m_dSize*1./m_dNum);
    for (int i{0}; i<m_dNum; i++) {
        qint64 start = perSize*i;
        qint64 end = perSize*(i+1) - 1;
        if (i == (m_dNum-1)) {
            end = start + m_dSize - perSize*i;
        }
        uint hasDownSize = 0;
        if (mHasDownSize.contains(i)) {
            hasDownSize = mHasDownSize[i];
        }
        Download *loader = new Download(i,this);
        m_mLoaders[i] = loader;
        connect(loader,&Download::sigWrite,this,&DownloadManager::slotWrite);
        loader->startDownload(m_dUrl,start,end,hasDownSize);
    }

    sets.remove(m_dUuid);
    sets.remove(m_dUuid+"-thread");
    size = sets.beginReadArray("FileID");
    bool flag = false;
    for (int i{0}; i<size; i++) {
        sets.setArrayIndex(i);
        QString id = sets.value("id").toString();
        if (id == m_dUuid) {
            sets.remove("id");
            flag = true;
            break;
        }
    }
    sets.endArray();
    if (flag) {
        sets.setValue("FileID/size",sets.value("FileID/size").toUInt()-1);
    }
    sets.sync();
    return true;
}

void DownloadManager::saveResumeInfo()
{
    for (auto it=m_mLoaders.begin(); it!=m_mLoaders.end(); it++) {
        it.value()->quit();
        it.value()->wait();
    }
    if (!m_bFinish && m_dNum > 1) {
        QString path = QApplication::applicationDirPath()+"/resume.ini";
        QSettings sets(path,QSettings::IniFormat);
        int size = sets.beginReadArray("FileID");
        bool flag = false;
        for (int i{0}; i<size; i++) {
            sets.setArrayIndex(i);
            QString id = sets.value("id").toString();
            if (id == m_dUuid) {
                flag = true;
                break;
            }
        }
        sets.endArray();
        if (!flag) {
            sets.beginWriteArray("FileID");
            sets.setArrayIndex(size);
            sets.setValue("id",m_dUuid);
            sets.endArray();
        }
        sets.setValue(m_dUuid+"/url",m_dUrl.url());
        sets.setValue(m_dUuid+"/size",m_dSize);
        sets.setValue(m_dUuid+"/multi",m_dNum>1);
        int hasDownSum = 0;
        sets.beginWriteArray(m_dUuid+"-thread");
        for (auto it=m_mLoaders.begin(); it!=m_mLoaders.end(); it++) {
            if (it.value()) {
                sets.setArrayIndex(it.value()->m_dIndex);
                sets.setValue("index",it.value()->m_dIndex);
                sets.setValue("hasDownSize",it.value()->m_dLastDownSize);
                hasDownSum += it.value()->m_dLastDownSize;
            }
        }
        sets.endArray();
        sets.setValue(m_dUuid+"/hasDownSize",hasDownSum);
    }
}

void DownloadManager::slotWrite(QByteArray arr, int start)
{
    if (!m_bFinish) {
        m_pFile->seek(start);
        m_pFile->write(arr);
        m_dHasDownSize += arr.size();
        emit value(m_dHasDownSize/1000/1000);
        if (m_dHasDownSize == m_dSize) {
            m_pFile->close();
            m_bFinish = true;
        }
    }
}
