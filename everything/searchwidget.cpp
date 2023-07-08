#include "searchwidget.h"
#include "ui_searchwidget.h"
#include <QDebug>
#include <QTimer>
#include <QMenu>
#include <QProcess>
#include "diskglobal.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    m_pModel = new DiskModel<FileInfo>(this);
    ui->tableView->setModel(m_pModel);
    ui->tableView->setModel(m_pModel);

    //�̶�QTableWidget�п�
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //�̶�QTableWidget�и�
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //��������Ӧ�п�
    //ResizeToContents����Ӧ���� Stretch����Ӧ����
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//�Զ������п�
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    //����������С(�ֶ�����ͷ���),ǰ���ǲ��ܰ�ÿһ�ж�����ΪStretch
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);
    //�����п�
    ui->tableView->setColumnWidth(1,450);
    //��������
    //ui->tableView->setSortingEnabled(true) ;
    //ui->tableView->sortByColumn(4,Qt::AscendingOrder);

    ui->label->setVisible(false);
    connect(DiskGlobal::ins(), &DiskGlobal::sigQueryResult, this, &SearchWidget::slotQueryResult);
    connect(DiskGlobal::ins(), &DiskGlobal::sigProgressInfo, this, &SearchWidget::slotProgressInfo);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::on_lineEdit_textEdited(const QString &text)
{
    Q_UNUSED(text)
    static bool bDelay = false;
    if (!bDelay) {
        m_pModel->clear();
        bDelay = true;
        QTimer::singleShot(500,this,[&](){
            QString text = ui->lineEdit->text();
            if (text.isEmpty()) {
                m_pModel->clear();
            }
            emit sigSearchChanged(text);
            bDelay = false;
        });
    }
}

void SearchWidget::slotQueryResult(const QString &con, qint64 id, const FileInfo &file)
{
    if (ui->lineEdit->text() != con) {
        return;
    }
    m_pModel->append(id,file);
}

void SearchWidget::slotProgressInfo(const QString &info)
{
    if (info.isEmpty() || info == "finish") {
        ui->label->setVisible(false);
    }
    else {
        if (!ui->label->isVisible())
            ui->label->setVisible(true);
        ui->label->setText(info);
    }
}

void SearchWidget::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    if (ui->tableView->currentIndex().isValid()) {
        QString name = m_pModel->data(m_pModel->index(ui->tableView->currentIndex().row(),0)).toString();
        QString path = m_pModel->data(m_pModel->index(ui->tableView->currentIndex().row(),1)).toString();
        QMenu *menu = new QMenu;
        menu->setAttribute(Qt::WA_DeleteOnClose);
        QAction *action = menu->addAction(QStringLiteral("���ļ�"));
#ifdef Q_OS_WIN
        path.replace("/","\\");
        connect(action,&QAction::triggered,this,[&](){
            QProcess pro;
            pro.startDetached("explorer", QStringList() << QString("%1\\%2").arg(path).arg(name));
        });
        action = menu->addAction(QStringLiteral("���ļ���"));
        connect(action,&QAction::triggered,this,[&](){
            QProcess pro;
            pro.startDetached("explorer", QStringList() << QString("/select,") << QString("%1\\%2").arg(path).arg(name));
        });
#endif
        menu->exec(QCursor::pos());
    }
}
