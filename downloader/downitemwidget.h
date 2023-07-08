#ifndef DOWNITEMWIDGET_H
#define DOWNITEMWIDGET_H

#include <QWidget>
#include <QUrl>
#include "download.h"

namespace Ui {
class DownItemWidget;
}

class DownItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownItemWidget(QWidget *parent = nullptr);
    ~DownItemWidget();
    void setUrl(const QString &uid, const QUrl &url);
    void setResumeInfo(const QString &uid, const QUrl &url, uint size, bool multi, uint hasDownSize);
signals:
    void sigRemove(const QString &uid);
private slots:
    void on_downBtn_clicked();
    void on_delBtn_clicked();
private:
    Ui::DownItemWidget *ui;
    QUrl m_dUrl;
    bool m_bMulti;
    int m_dNum;
    QString m_dName;
    uint m_dSize;
    DownloadManager *m_pDown;
    bool m_bInit;
    QString m_dUuid;
};

#endif // DOWNITEMWIDGET_H
