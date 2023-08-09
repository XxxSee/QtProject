#ifndef FLOWCHART_H
#define FLOWCHART_H

#include <QWidget>

namespace Ui {
class FlowChart;
}

class FlowChart : public QWidget
{
    Q_OBJECT
public:
    explicit FlowChart(QWidget *parent = nullptr);
    ~FlowChart();
private slots:
    void on_execBtn_clicked();
    void on_clearBtn_clicked();
    void on_beginBtn_clicked();
    void on_endBtn_clicked();
    void on_openBtn_clicked();
    void on_grayBtn_clicked();
    void on_robertsBtn_clicked();
    void on_noiseBtn_clicked();
    void on_filterBtn_clicked();
    void on_affineBtn_clicked();
    void on_threshBtn_clicked();
    void on_matchBtn_clicked();
    void on_extractBtn_clicked();
    void on_cameraBtn_clicked();
private:
    Ui::FlowChart *ui;
};

#endif // FLOWCHART_H
