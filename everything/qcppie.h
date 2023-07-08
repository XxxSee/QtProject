#ifndef QCPPIE_H
#define QCPPIE_H

#include "qcustomplot.h"
#include <QThread>

class QCP_LIB_DECL QCPPieData
{
public:
  QCPPieData(){}
  QCPPieData(QString key, double value, QColor color):key(key),value(value),color(color){}
  QString key;
  double value;
  QColor color;
};
Q_DECLARE_TYPEINFO(QCPPieData, Q_MOVABLE_TYPE);

typedef QMap<QString, QCPPieData> QCPPieDataMap;
typedef QMapIterator<QString, QCPPieData> QCPPieDataMapIterator;
typedef QMutableMapIterator<QString, QCPPieData> QCPPieDataMutableMapIterator;

class QCP_LIB_DECL QCPSlice
{
public:
    QCPSlice() {}
    QCPSlice(const QString &key, const double &value, const QColor &color) : key(key),value(value),color(color) {}
    ~QCPSlice() {}
    QString key;
    double value;
    QColor color;
    QPainterPath outPath;
    QPainterPath inPath;
    QPointF p1,p2,p3;
    QRectF textRect;
    int textAligns;
    float textSize;
    QString text;
};
Q_DECLARE_TYPEINFO(QCPSlice, Q_MOVABLE_TYPE);

typedef QMap<QString, QCPSlice> QCPSliceMap;
typedef QMapIterator<QString, QCPSlice> QCPSliceMapIterator;
typedef QMutableMapIterator<QString, QCPSlice> QCPSliceMutableMapIterator;

class QTimer;
class QCP_LIB_DECL QCPPie : public QCPAbstractPlottable
{
    Q_OBJECT
public:
    enum LabelType {
        ltNone,
        ltHole,
        ltInsideRatioWidth,
        ltInsideRatioHeight,
        ltOutsideNoWithLine,
        ltOutsideWithLine,
        ltInsideAndHole,
        ltOutsideAndHole,
    };
    enum PositionType {
        ptPlotCoords,
        ptPlotCoordsRatio,
    };

    explicit QCPPie(QCPAxis *keyAxis, QCPAxis *valueAxis);
    ~QCPPie();

    // getters:
    QCPPieDataMap *data() const { return mData; }

    // setters:
    void setData(QCPPieDataMap *data, bool copy=false);
    void setData(const QVector<QString> &key, const QVector<double> &value, const QVector<QColor> &color=QVector<QColor>());

    // non-property methods:
    void addData(const QCPPieDataMap &dataMap);
    void addData(const QCPPieData &data);
    void addData(QString key, double value, QColor color=QColor());
    void addData(const QVector<QString> &keys, const QVector<double> &values, const QVector<QColor> &colors=QVector<QColor>());
    void removeDataBefore(QString key);
    void removeDataAfter(QString key);
    void removeData(QString fromKey, QString toKey);
    void removeData(QString key);

    // reimplemented virtual methods:
    virtual void clearData();
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

    // property methods:
    void setRadius(double radius) { if (radius > 0) mRadius=radius; }
    void setHoleSize(double size) { if (size >=0 && size<1) mHoleSize=size; }
    void setCenter(double x, double y) { mCenterX=x;mCenterY=y; }
    void setLabelType(LabelType lt) { mLabelType=lt; }
    void setLabelSize(double size) { if (size > 0.) mLabelSize=size; }
    // {key}-key {value}-value {percent}-percent
    void setLabelFormat(const QString &format) { mLabelFormat=format; }
    void setPieName(const QString &name) { mPieName=name; setName(name); }
    void setPositionType(PositionType pt) { mPositionType=pt; }
signals:
    void sliceSelected(const QString &key, bool selected);
protected:
    // property members:
    QCPPieDataMap *mData;
    double mRadius;
    double mHoleSize;
    double mLabelSize;
    double mSum;
    double mCenterX;
    double mCenterY;
    QString mCurSlice;
    LabelType mLabelType;
    QString mLabelFormat;
    QRectF mOutPixelRect;
    QRectF mInPixelRect;
    QString mPieName;
    bool mHoleText;
    PositionType mPositionType;
    double mRatioOffset;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter);
    virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
    virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
    virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;

    //non-virtual methods:
    QRectF getSliceTextRectF(QRectF originRect, double beginAngle, double angle);
    void getSlice(QCPSlice &slice, double beginAngle, double angle, double radius, double scale);
    void drawSlice(QCPPainter *painter, QCPSlice &slice);
    void drawLine(QCPPainter *painter, QCPSlice &slice);
    void drawText(QCPPainter *painter, QCPSlice &slice);
    QColor getRandomColor();
    double getPoint2PointAngle(QPointF p1, QPointF p2, QPointF center);

    //slot methods:
    void slotMousePress(QMouseEvent *event);


    friend class QCustomPlot;
    friend class QCPLegend;
};

#endif // QCPPIE_H
