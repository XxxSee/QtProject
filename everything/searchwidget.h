#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "diskmodel.h"

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();
signals:
    void sigSearchChanged(const QString &text);
private slots:
    void on_lineEdit_textEdited(const QString &text);
    void slotQueryResult(const QString &con, qint64 id, const FileInfo &file);
    void slotProgressInfo(const QString &info);
    void on_tableView_customContextMenuRequested(const QPoint &pos);
private:
    Ui::SearchWidget *ui;
    DiskModel<FileInfo> *m_pModel;
};

#endif // SEARCHWIDGET_H
