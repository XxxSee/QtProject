#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QPointer>
#include <QThread>

class QNetworkReply;
class QFile;

class Download : public QThread
{
    Q_OBJECT
public:
    explicit Download(int index, QObject *parent = nullptr);
    ~Download();
    void startDownload(const QUrl &url, uint start, uint end, uint hasDown = 0);

    int m_dIndex;
    QUrl m_dUrl;
    uint m_dStart;
    uint m_dEnd;
    uint m_dHasDownSize;
    uint m_dLastDownSize;
    uint m_dStartOffet;
signals:    
    void sigWrite(QByteArray arr, int start);
protected:
    void run() override;
};

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    DownloadManager(const QUrl &url, const QString &name, uint size, int num, const QString &uid, bool resume, QObject *parent = nullptr);
    ~DownloadManager();
    bool start();
    void pause(bool b);
    void remove();
signals:
    void finished();
    void range(int, int);
    void value(int);
private:
    bool startNew();
    bool startResume();
    void saveResumeInfo();
private slots:
    void slotWrite(QByteArray arr, int start);
private:
    QUrl m_dUrl;
    QString m_dName;
    QString m_dUuid;
    uint m_dSize;
    int m_dNum;
    uint m_dHasDownSize;
    QFile *m_pFile;
    QMap<int,Download*> m_mLoaders;
    bool m_bResume;
    bool m_bFinish;
};

#endif // DOWNLOAD_H
