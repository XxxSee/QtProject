#include "chartwidget.h"
#include "ui_chartwidget.h"
#include <QRandomGenerator>
#include <QSharedPointer>
#include "diskglobal.h"

ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartWidget),
    m_dCT(CT_Pie)
{
    ui->setupUi(this);

    connect(DiskGlobal::ins(), &DiskGlobal::sigSelectResult, this, &ChartWidget::slotSelectResult);
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::reset(ChartType ct)
{
    m_dCT = ct;
    delete ui->customPlot;
    ui->customPlot = new QCustomPlot(ui->widget);
    ui->gridLayout_2->addWidget(ui->customPlot);
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    ui->customPlot->legend->setSelectableParts(QCPLegend::spItems);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_mPies.clear();
    m_lDisks.clear();
    m_mBars.clear();
    m_mLabels.clear();
    m_mDatas.clear();

    if (m_dCT == CT_Bar) {
        m_pBarGrp = new QCPBarsGroup(ui->customPlot);
        ui->customPlot->xAxis->setLabel(QStringLiteral("扩展名"));
        ui->customPlot->yAxis->setLabel(QStringLiteral("数量"));
    }
}

void ChartWidget::slotSelectResult(const QString &disk, const QString &suffix, qint64 count)
{
    if (m_dCT == CT_Pie) {
        if (disk == "finish") {
            updatePie();
        }
        else {
            m_mDatas[disk][suffix] = count;
            m_mDatas["System"][suffix] +=  count;
        }
    }
    else if (m_dCT == CT_Bar) {
        if (disk == "finish") {
            updateBar();
        }
        else {
            m_mDatas[disk][suffix] = count;
            m_mLabels[suffix] = 0;
            m_mDatas["System"][suffix] +=  count;
        }
    }
}

void ChartWidget::updatePie()
{
    m_lDisks = m_mDatas.keys();
    m_lDisks.push_back("OutDisk");
    m_lDisks.push_back("InSystem");
    for (int i{0}; i<m_lDisks.size(); i++) {
        /*
         *  0   1
         *           4
         *  2   3
         */
        QCPPie *pie = new QCPPie(ui->customPlot->xAxis, ui->customPlot->yAxis);
        ui->customPlot->addPlottable(pie);
        m_mPies[m_lDisks[i]] = pie;
        if (i == (m_lDisks.size() - 2)) {
            int x = 10;
            int y = 5;
            pie->setCenter(x,y);
            pie->setRadius(3);
            pie->setHoleSize(0.7);
            pie->setLabelSize(2);
            pie->setLabelType(QCPPie::ltInsideRatioWidth);
        }
        else if (i == (m_lDisks.size() - 1)) {
            int x = 10;
            int y = 5;
            pie->setCenter(x,y);
            pie->setRadius(1);
            pie->setLabelSize(4);
            pie->setLabelType(QCPPie::ltInsideRatioWidth);
        }
        else {
            int r = i%2;
            int c = i/2;
            int x = r*3 + 1;
            int y = 6 - c*3;
            pie->setCenter(x,y);
            pie->setRadius(1);
            pie->setHoleSize(0.6);
            pie->setLabelSize(1);
            pie->setLabelType(QCPPie::ltInsideAndHole);
        }
        pie->setPositionType(QCPPie::ptPlotCoordsRatio);
        pie->setLabelFormat("{key}:{percent}");
        pie->setPieName(QString("%1 Pie").arg(m_lDisks[i]));
    }
    QMap<QString,qint64> InSystemData;
    double cnt = 0;
    for (int i=0; i<m_lDisks.size()-2; i++) {
        QMap<QString,qint64> &data = m_mDatas[m_lDisks[i]];
        QCPPie *pie = m_mPies[m_lDisks[i]];
        cnt = 0;
        for (auto it=data.cbegin(); it!=data.cend(); it++) {
            cnt += it.value();
            pie->addData(it.key(),it.value());
        }
        if (m_lDisks[i] != "System")
            InSystemData[m_lDisks[i]] = cnt;
    }
    QCPPie *pie = m_mPies["InSystem"];
    connect(pie,&QCPPie::sliceSelected,this,[&](const QString &disk, bool select){
        QString curDisk = select ? disk : "System";
        QCPPie *pie = m_mPies["OutDisk"];
        pie->clearData();
            QMap<QString,qint64> &data = m_mDatas[curDisk];
            for (auto it=data.begin(); it!=data.end(); it++) {
            pie->addData(it.key(),it.value());
        }
        ui->customPlot->replot();
    });
    for (auto it=InSystemData.begin(); it!=InSystemData.end(); it++) {
        qDebug() << it.key() << it.value();
        pie->addData(it.key(),it.value());
    }
    pie = m_mPies["OutDisk"];
    QMap<QString,qint64> &data = m_mDatas["System"];
    for (auto it=data.begin(); it!=data.end(); it++) {
        pie->addData(it.key(),it.value());
    }
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}

void ChartWidget::updateBar()
{
    QList<QString> disks = m_mDatas.keys();
    for (int i{0}; i<disks.size(); i++) {
        QCPBars *bar = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
        ui->customPlot->addPlottable(bar);
        m_mBars[disks[i]] = bar;
        bar->setName(disks[i]);
        bar->setShowValue(true);
        m_pBarGrp->append(bar);
        m_pBarGrp->setSpacingType(QCPBarsGroup::stAbsolute);  //按像素设置组内柱状图的间距
        m_pBarGrp->setSpacing(5); //设置较小的间距值
        QColor brushColor(QRandomGenerator::global()->bounded(0,255),QRandomGenerator::global()->bounded(0,255),QRandomGenerator::global()->bounded(0,255));
        bar->setPen(Qt::NoPen);
        bar->setBrush(brushColor);
    }
    foreach (QCPBars *bar, m_mBars) {
        //以key坐标轴计算宽度的大小
        bar->setWidthType(QCPBars::wtPlotCoords);
        bar->setWidth(bar->width() / m_mBars.size()); //柱状图的宽度大小
    }
    int key = 1;
    QVector<double> ticks;
    for (auto it=m_mLabels.begin(); it!=m_mLabels.end(); it++) {
        for (int i{0}; i<disks.size(); i++) {
            QMap<QString,qint64> &datas = m_mDatas[disks[i]];
            int cnt = datas[it.key()];
            m_mBars[disks[i]]->addData(key,cnt);
            //修改QCPBars类，在其中绘制count
            //if (cnt > 0) {
            //    QCPItemText *itemText = new QCPItemText(ui->customPlot);
            //    itemText->setClipToAxisRect(false);
            //    //设置itemText的位置是跟随坐标系的
            //    itemText->position->setType(QCPItemPosition::ptPlotCoords);
            //    //设置itemText跟随的坐标系为 ui->customplot->xAxis, ui->customplot->yAxis
            //    itemText->position->setAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
            //    //设置itemText放置的位置
            //    itemText->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
            //    //设置itemText显示的内容
            //    itemText->setText(QString::number(cnt));
            //    //设置itemText显示的位置的坐标
            //    itemText->position->setCoords(key, cnt);
            //}
        }
        ticks << key;
        key++;
    }
    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(m_mLabels.keys().toVector());
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    m_mLabels.clear();
    m_mDatas.clear();
}
