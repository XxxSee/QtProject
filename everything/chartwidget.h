#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QHash>
#include <QSet>
#include "qcppie.h"

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    enum ChartType { CT_Pie, CT_Bar };
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();
    void reset(ChartType ct);
signals:
    void sigSelectTable(const QString &exec);
private slots:
    void slotSelectResult(const QString &disk, const QString &suffix, qint64 count);
private:
    void updatePie();
    void updateBar();
private:
    Ui::ChartWidget *ui;
    ChartType m_dCT;
    QMap<QString,QCPPie*> m_mPies;
    QList<QString> m_lDisks;
    QMap<QString,QCPBars*> m_mBars;
    QMap<QString,double> m_mLabels;
    QMap<QString,QMap<QString,qint64>> m_mDatas;
    QCPBarsGroup *m_pBarGrp;
};
#endif // CHARTWIDGET_H
