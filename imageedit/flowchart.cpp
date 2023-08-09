#include "flowchart.h"
#include "ui_flowchart.h"
#include "component.h"

FlowChart::FlowChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlowChart)
{
    ui->setupUi(this);
}

FlowChart::~FlowChart()
{
    delete ui;
}

void FlowChart::on_execBtn_clicked()
{
    ui->graphicsView->startExec();
}

void FlowChart::on_clearBtn_clicked()
{
    ui->graphicsView->clearResult();
}

void FlowChart::on_beginBtn_clicked()
{
    ui->graphicsView->addItem<ctBegin>();
}

void FlowChart::on_endBtn_clicked()
{
    ui->graphicsView->addItem<ctEnd>();
}

void FlowChart::on_openBtn_clicked()
{
    ui->graphicsView->addItem<ctOpen>();
}

void FlowChart::on_grayBtn_clicked()
{
    ui->graphicsView->addItem<ctGray>();
}

void FlowChart::on_robertsBtn_clicked()
{
    ui->graphicsView->addItem<ctRobertsEdge>();
}

void FlowChart::on_noiseBtn_clicked()
{
    ui->graphicsView->addItem<ctSaltNoise>();
}

void FlowChart::on_filterBtn_clicked()
{
    ui->graphicsView->addItem<ctAverageFilter>();
}

void FlowChart::on_affineBtn_clicked()
{
    ui->graphicsView->addItem<ctAffine>();
}

void FlowChart::on_threshBtn_clicked()
{
    ui->graphicsView->addItem<ctThresholdSeg>();
}

void FlowChart::on_matchBtn_clicked()
{
    ui->graphicsView->addItem<ctCircleLBP>();
}

void FlowChart::on_extractBtn_clicked()
{
    ui->graphicsView->addItem<ctSIFT>();
}

void FlowChart::on_cameraBtn_clicked()
{
    ui->graphicsView->addItem<ctCameraLock>();
}
