#include "qcppie.h"
#include <QVector2D>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QPolygonF>

QCPPie::QCPPie(QCPAxis *keyAxis, QCPAxis *valueAxis) :
    QCPAbstractPlottable(keyAxis,valueAxis),
    mRadius(1.),
    mHoleSize(0.0),
    mLabelSize(2),
    mSum(0.),
    mCenterX(0),
    mCenterY(0),
    mLabelType(ltNone),
    mHoleText(false),
    mPositionType(ptPlotCoords),
    mRatioOffset(0)
{
    mData = new QCPPieDataMap;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    connect(keyAxis->parentPlot(),&QCustomPlot::mousePress,this,&QCPPie::slotMousePress);
}

QCPPie::~QCPPie()
{
    delete mData;
}

void QCPPie::setData(QCPPieDataMap *data, bool copy)
{
    if (mData == data)
    {
      qDebug() << Q_FUNC_INFO << "The data pointer is already in (and owned by) this plottable" << reinterpret_cast<quintptr>(data);
      return;
    }
    if (copy)
    {
      *mData = *data;
    } else
    {
      delete mData;
      mData = data;
    }
    mSum = 0;
    QCPPieDataMapIterator it(*mData);
    while (it.hasNext()) {
        it.next();
        mSum += it.value().value;
    }
}

void QCPPie::setData(const QVector<QString> &keys, const QVector<double> &values, const QVector<QColor> &color)
{
    mData->clear();
    mSum = 0;
    int n = keys.size();
    n = qMin(n, values.size());
    QCPPieData newData;
    if (color.size() >= n) {
        for (int i=0; i<n; ++i)
        {
            if (mData->contains(keys[i])) {
                QCPPieData data = mData->value(keys[i]);
                data.value += values[i];
                mData->insert(data.key, data);
            }
            else {
                newData.key = keys[i];
                newData.value = values[i];
                newData.color = color[i];
                mData->insertMulti(newData.key, newData);
            }
            mSum += values[i];
        }
    }
    else {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        for (int i=0; i<n; ++i)
        {
            if (mData->contains(keys[i])) {
                QCPPieData data = mData->value(keys[i]);
                data.value += values[i];
                mData->insert(data.key, data);
            }
            else {
                newData.key = keys[i];
                newData.value = values[i];
                newData.color = getRandomColor();
                mData->insert(newData.key, newData);
            }
            mSum += values[i];
        }
    }
}

void QCPPie::addData(const QCPPieDataMap &dataMap)
{
    //mData->unite(dataMap);
    QCPPieDataMapIterator it(dataMap);
    while (it.hasNext()) {
        it.next();
        if (mData->contains(it.key())) {
            QCPPieData data = mData->value(it.key());
            data.value += it.value().value;
            mData->insert(it.key(),data);
        }
        else {
            mData->insert(it.key(),it.value());
        }
        mSum += it.value().value;
    }
}

void QCPPie::addData(const QCPPieData &data)
{
    if (mData->contains(data.key)) {
        QCPPieData d = mData->value(data.key);
        d.value += data.value;
        mData->insert(d.key,d);
    }
    else {
        mData->insert(data.key,data);
    }
    mSum += data.value;
}

void QCPPie::addData(QString key, double value, QColor color)
{
    if (mData->contains(key)) {
        QCPPieData data = mData->value(key);
        data.value += value;
        mData->insert(key, data);
    }
    else {
        QCPPieData newData;
        newData.key = key;
        newData.value = value;
        if (color.isValid()) {
            newData.color = color;
        }
        else {
            newData.color = getRandomColor();
        }
        mData->insert(newData.key, newData);
    }
    mSum += value;
}

void QCPPie::addData(const QVector<QString> &keys, const QVector<double> &values, const QVector<QColor> &colors)
{
    int n = qMin(keys.size(), values.size());
    QCPPieData newData;
    if (colors.size() >= n) {
        for (int i=0; i<n; ++i)
        {
            if (mData->contains(keys[i])) {
                QCPPieData data = mData->value(keys[i]);
                data.value += values[i];
                mData->insert(data.key, data);
            }
            else {
                newData.key = keys[i];
                newData.value = values[i];
                newData.color = colors[i];
                mData->insert(newData.key, newData);
            }
            mSum += values[i];
        }
    }
    else {
        for (int i=0; i<n; ++i)
        {
            if (mData->contains(keys[i])) {
                QCPPieData data = mData->value(keys[i]);
                data.value += values[i];
                mData->insert(data.key, data);
            }
            else {
                newData.key = keys[i];
                newData.value = values[i];
                newData.color = getRandomColor();
                mData->insert(newData.key, newData);
            }
            mSum += values[i];
        }
    }
}

void QCPPie::removeDataBefore(QString key)
{
    QCPPieDataMap::iterator it = mData->begin();
    while (it != mData->end() && it.key() < key) {
        mSum -= it.value().value;
        it = mData->erase(it);
    }
}

void QCPPie::removeDataAfter(QString key)
{
    if (mData->isEmpty()) return;
    QCPPieDataMap::iterator it = mData->upperBound(key);
    while (it != mData->end()) {
        mSum -= it.value().value;
        it = mData->erase(it);
    }
}

void QCPPie::removeData(QString fromKey, QString toKey)
{
    if (fromKey >= toKey || mData->isEmpty()) return;
    QCPPieDataMap::iterator it = mData->upperBound(fromKey);
    QCPPieDataMap::iterator itEnd = mData->upperBound(toKey);
    while (it != itEnd) {
        mSum -= it.value().value;
        it = mData->erase(it);
    }
}

void QCPPie::removeData(QString key)
{
    mSum -= mData->value(key).value;
    mData->remove(key);
}

void QCPPie::clearData()
{
    mCurSlice = "";
    mHoleText = false;
    mSum = 0;
    mData->clear();
}

double QCPPie::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if ((onlySelectable && !mSelectable) || mData->isEmpty())
      return -1;
    if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }

    if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
      return -1;
    else
        return -1;
}

void QCPPie::draw(QCPPainter *painter)
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
    if (mKeyAxis.data()->range().size() <= 0 || mData->isEmpty()) return;

    if (mPositionType == ptPlotCoordsRatio) {
        double ratio = parentPlot()->axisRect()->width()*1. / parentPlot()->axisRect()->height();
        mRatioOffset = ratio*mCenterY;
    }
    else {
        mRatioOffset = mCenterY;
    }
    double x1 = keyAxis->coordToPixel(mCenterX);
    double y1 = valueAxis->coordToPixel(mRatioOffset);
    double x2 = keyAxis->coordToPixel(mCenterX+mRadius);
    double y2 = valueAxis->coordToPixel(mRatioOffset);
    double radius = QVector2D(x2-x1,y2-y1).length();
    double scale = radius*1./mRadius;
    mOutPixelRect = QRectF(x1-radius,y1-radius,2*radius,2*radius);
    //mInPixelRect = QRectF(QPointF(x1-radius*mHoleSize,y1-radius*mHoleSize),QSize(2*radius*mHoleSize,2*radius*mHoleSize));
    mInPixelRect = mOutPixelRect.marginsRemoved(QMarginsF((1.-mHoleSize)*radius,(1.-mHoleSize)*radius,(1.-mHoleSize)*radius,(1.-mHoleSize)*radius));
    mHoleText = false;

    QCPPieDataMap::const_iterator it, itStart, itEnd;
    itStart = mData->cbegin();
    itEnd = mData->cend();
    double beginAngle = 0, angle = 0;
    for (it=itStart; it!=itEnd; it++) {
        QCPSlice slice;
        slice.key = it.value().key;
        slice.color = it.value().color;
        slice.value = it.value().value;
        angle = it.value().value*360./mSum;
        //if (angle < 0.1) {
        //    qDebug() << "splice count is too small that will not show";
        //}
        applyFillAntialiasingHint(painter);
        getSlice(slice,beginAngle,angle,radius,scale);
        drawSlice(painter,slice);
        drawLine(painter,slice);
        drawText(painter,slice);
        beginAngle += angle;
    }

}

void QCPPie::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
    // draw fill:
    if (mBrush.style() != Qt::NoBrush)
    {
      applyFillAntialiasingHint(painter);
      painter->fillRect(QRectF(rect.left(), rect.top()+rect.height()/2.0, rect.width(), rect.height()/3.0), mBrush);
    }
    if (!mCurSlice.isEmpty()) {
        QCPPieData d = mData->value(mCurSlice);
        painter->save();
        painter->setBrush(d.color);
        painter->drawRect(QRectF(rect.left()+rect.width()/4, rect.top()+rect.width()/4, rect.width()/2, rect.height()/2));
    }
}

QCPRange QCPPie::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
    QCPRange range;
    bool lower = false;
    bool upper = false;
    if (inSignDomain == sdNegative) {

    }
    else if (inSignDomain == sdPositive) {

    }
    else if (inSignDomain == sdBoth) {
        range.lower = 0;
        range.upper = mCenterX+mRadius;
        lower = true;
        upper = true;
    }
    foundRange = lower & upper;
    return range;
}

QCPRange QCPPie::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
    QCPRange range;
    bool lower = false;
    bool upper = false;
    if (inSignDomain == sdNegative) {

    }
    else if (inSignDomain == sdPositive) {

    }
    else if (inSignDomain == sdBoth) {
        if (mPositionType == ptPlotCoordsRatio) {
            double ratio = parentPlot()->axisRect()->width()*1. / parentPlot()->axisRect()->height();
            range.upper = ratio*mCenterY+ratio*mRadius;
            range.lower = 0;//ratio*mCenterY+mRadius;
        }
        else {
            range.upper = mCenterY+mRadius;
            range.lower = 0;
        }
        lower = true;
        upper = true;
    }
    foundRange = lower & upper;
    return range;
}

QRectF QCPPie::getSliceTextRectF(QRectF originRect, double beginAngle, double angle)
{
    QPointF tl = originRect.topLeft();
    QPointF tr = originRect.topRight();
    QPointF bl = originRect.bottomLeft();
    QPointF br = originRect.bottomRight();
    tl = QPointF((tl.x()-mOutPixelRect.center().x())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                -(tl.y()-mOutPixelRect.center().y())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().x(),
                 (tl.y()-mOutPixelRect.center().y())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                +(tl.x()-mOutPixelRect.center().x())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().y());
    tr = QPointF((tr.x()-mOutPixelRect.center().x())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                -(tr.y()-mOutPixelRect.center().y())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().x(),
                 (tr.y()-mOutPixelRect.center().y())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                +(tr.x()-mOutPixelRect.center().x())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().y());
    bl = QPointF((bl.x()-mOutPixelRect.center().x())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                -(bl.y()-mOutPixelRect.center().y())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().x(),
                 (bl.y()-mOutPixelRect.center().y())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                +(bl.x()-mOutPixelRect.center().x())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().y());
    br = QPointF((br.x()-mOutPixelRect.center().x())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                -(br.y()-mOutPixelRect.center().y())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().x(),
                 (br.y()-mOutPixelRect.center().y())*qCos(qDegreesToRadians(90-beginAngle-angle/2.))
                +(br.x()-mOutPixelRect.center().x())*qSin(qDegreesToRadians(90-beginAngle-angle/2.))+mOutPixelRect.center().y());

    QPolygonF poly;
    poly << tl << bl << br << tr;
    return poly.boundingRect();
}


void QCPPie::getSlice(QCPSlice &slice, double beginAngle, double angle, double radius, double scale)
{
    double midAng = beginAngle + angle/2.;
    QRectF paintRect = mOutPixelRect;
    double selectOffset = 0;
    //when one slice is checked
    if (mCurSlice == slice.key) {
        selectOffset = radius*(1-mHoleSize);
        paintRect = mOutPixelRect.marginsAdded(QMarginsF(selectOffset,selectOffset,selectOffset,selectOffset));
    }

    //slice path
    slice.outPath.moveTo(paintRect.center());
    slice.outPath.arcTo(paintRect,beginAngle,angle);
    if (!qFuzzyIsNull(mHoleSize)) {
        slice.inPath.moveTo(mInPixelRect.center());
        //beginAngle-1.,angle+2.ÊÇÎªÁËsubtracted¼õÈ¥µÄ¸ü¸É¾»
        slice.inPath.arcTo(mInPixelRect,beginAngle-1.,angle+2.);
    }

    //slice label text rect and alignment
    slice.textSize = mLabelSize*scale/25.;
    //qDebug() << mPieName << mLabelSize << slice.textSize;
    //label text format
    if (mLabelFormat.isEmpty()) {
        slice.text = slice.key;
    }
    else {
        QString text = mLabelFormat;
        text.replace("{key}",slice.key);
        text.replace("{value}",QString::number(slice.value));
        QString percentStr = QString("%1%").arg(slice.value*100/mSum,0,'g',2);
        text.replace("{percent}",percentStr);
        slice.text = text;
    }
    if (mLabelType == ltHole) {
        slice.textRect = mInPixelRect;
        slice.textAligns = Qt::AlignCenter;
    }
    else if (mLabelType == ltInsideRatioWidth || mLabelType == ltInsideAndHole) {
        QPainterPath path(paintRect.center());
        path.arcTo(paintRect,90-angle/2.,angle);
        QRectF originRect = QRectF(paintRect.center().x()-path.boundingRect().width()/2.,paintRect.top(),path.boundingRect().width(),radius*(1-mHoleSize)+selectOffset);
        slice.textRect = getSliceTextRectF(originRect,beginAngle,angle);
        slice.textAligns = Qt::AlignCenter;
    }
    else if (mLabelType == ltInsideRatioHeight) {
        QRectF originRect = QRectF(paintRect.center().x()-radius*(1-mHoleSize)/2.,paintRect.top(),radius*(1-mHoleSize),radius*(1-mHoleSize));
        slice.textRect = getSliceTextRectF(originRect,beginAngle,angle);
        slice.textAligns = Qt::AlignCenter;
    }
    else if (mLabelType == ltOutsideNoWithLine) {
        QFontMetrics fontMetrics(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize));
        QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|Qt::AlignCenter,slice.text);
        QPainterPath path(paintRect.center());
        path.arcTo(paintRect,90-angle/2.,angle);
        QRectF originRect = QRectF(paintRect.center().x()-textRect.width()/2.,paintRect.top()-radius*(1-mHoleSize),textRect.width(),radius*(1-mHoleSize));
        slice.textRect = getSliceTextRectF(originRect,beginAngle,angle);
        slice.textAligns = Qt::AlignCenter;
    }
    //label line points
    else if (mLabelType == ltOutsideWithLine || mLabelType == ltOutsideAndHole) {
        double dx1 = qCos(qDegreesToRadians(-midAng))*(radius+selectOffset);
        double dy1 = qSin(qDegreesToRadians(-midAng))*(radius+selectOffset);
        double dx2 = qCos(qDegreesToRadians(-midAng))*(radius*2+selectOffset);
        double dy2 = qSin(qDegreesToRadians(-midAng))*(radius*2+selectOffset);
        slice.p1 = mOutPixelRect.center()+QPointF(dx1,dy1);
        slice.p2 = mOutPixelRect.center()+QPointF(dx2,dy2);
        int fac = (slice.p2.x()-slice.p1.x()) > 0 ? 1 : (slice.p2.x()-slice.p1.x()) < 0 ? -1 : 1;
        slice.p3 = QPointF(slice.p2.x()+scale/10.*fac,slice.p2.y());
        QPainterPath path(mOutPixelRect.center());
        path.arcTo(mOutPixelRect,90-angle/2.,angle);
        if (fac == 1) {
            QFontMetrics fontMetrics(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize));
            QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|Qt::AlignLeft|Qt::AlignVCenter,slice.text);
            slice.textRect = QRectF(QPointF(slice.p3.x(),slice.p3.y()-radius*(1-mHoleSize)/2.),QSizeF(textRect.width(),radius*(1-mHoleSize)));
            slice.textAligns = Qt::AlignLeft|Qt::AlignVCenter;
        }
        else if (fac == -1) {
            QFontMetrics fontMetrics(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize));
            QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|Qt::AlignRight|Qt::AlignVCenter,slice.text);
            slice.textRect = QRectF(QPointF(slice.p3.x()-textRect.width(),slice.p3.y()-radius*(1-mHoleSize)/2.),QSizeF(textRect.width(),radius*(1-mHoleSize)));
            slice.textAligns = Qt::AlignRight|Qt::AlignVCenter;
        }
    }
}

void QCPPie::drawSlice(QCPPainter *painter,  QCPSlice &slice)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(slice.color);
    if (qFuzzyIsNull(mHoleSize)) {
        painter->drawPath(slice.outPath);
    }
    else {
        painter->drawPath(slice.outPath.subtracted(slice.inPath));
    }    
}

void QCPPie::drawLine(QCPPainter *painter, QCPSlice &slice)
{
    if (mLabelType == ltOutsideWithLine || mLabelType == ltOutsideAndHole) {
        painter->setPen(QPen(slice.color));
        painter->drawLine(QLineF(slice.p1,slice.p2));
        painter->drawLine(QLineF(slice.p2,slice.p3));
    }
}

void QCPPie::drawText(QCPPainter *painter, QCPSlice &slice)
{
    //painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination); //ÉèÖÃ»ìºÏÄ£Ê½
    painter->setPen(QPen(QColor(255-slice.color.red(),255-slice.color.green(),255-slice.color.blue())));
    painter->setFont(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize));
    if (mLabelType == ltHole) {
        painter->setFont(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize*2));
        if (mCurSlice.isEmpty() && !mHoleText) {
            mHoleText = true;
            painter->setPen(Qt::black);
            painter->drawText(mInPixelRect,Qt::AlignCenter,mPieName);
        }
        else if (mCurSlice == slice.key) {
            painter->setPen(Qt::black);
            painter->drawText(mInPixelRect,Qt::AlignCenter,slice.text);
        }
    }
    if (mLabelType == ltInsideRatioWidth || mLabelType == ltInsideRatioHeight
        || mLabelType == ltOutsideNoWithLine) {
        if (slice.value*100./mSum > 4 || mCurSlice == slice.key) {
            painter->setPen(QPen(QColor(255-slice.color.red(),255-slice.color.green(),255-slice.color.blue())));
            painter->drawText(slice.textRect,slice.textAligns,slice.text);
        }
    }
    else if (mLabelType == ltOutsideWithLine) {
       painter->setPen(slice.color);
       painter->drawText(slice.textRect,slice.textAligns,slice.text);
    }
    else if (mLabelType == ltInsideAndHole) {
        if (slice.value*100./mSum > 4) {
            painter->setPen(QPen(QColor(255-slice.color.red(),255-slice.color.green(),255-slice.color.blue())));
            painter->drawText(slice.textRect,slice.textAligns,slice.text);
        }
        painter->setFont(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize*2));
        if (mCurSlice.isEmpty() && !mHoleText) {
            mHoleText = true;
            painter->setPen(Qt::black);
            painter->drawText(mInPixelRect,Qt::AlignCenter,mPieName);
        }
        else if (mCurSlice == slice.key) {
            painter->setPen(slice.color);
            painter->drawText(mInPixelRect,Qt::AlignCenter,slice.text);
        }
    }
    else if (mLabelType == ltOutsideAndHole) {
        painter->setPen(slice.color);
        painter->drawText(slice.textRect,slice.textAligns,slice.text);
        painter->setFont(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"),slice.textSize*2));
        if (mCurSlice.isEmpty() && !mHoleText) {
            mHoleText = true;
            painter->setPen(Qt::black);
            painter->drawText(mInPixelRect,Qt::AlignCenter,mPieName);
        }
        else if (mCurSlice == slice.key) {
            painter->setPen(slice.color);
            painter->drawText(mInPixelRect,Qt::AlignCenter,slice.text);
        }
    }
}

QColor QCPPie::getRandomColor()
{
    QVector<int> rgb;
    for (int i{0}; i<3; i++) {
        //qsrand(QDateTime::currentMSecsSinceEpoch());
        rgb.push_back((qrand()*333)%255);
        //QThread::msleep(1);
    }
    return QColor(rgb[0],rgb[1],rgb[2]);
}

double QCPPie::getPoint2PointAngle(QPointF p1, QPointF p2, QPointF center)
{
    double x1 = p1.x(), y1 = p1.y(), x2 = p2.x(), y2 = p2.y(), x3 = center.x(), y3 = center.y();
    double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;
    theta = theta * 180.0 / M_PI;
    if (theta < 0) {
        theta = 180. + 180. + theta;
    }
    if (theta > 360.) {
        theta = theta - 360.;
    }
    return theta;
}


void QCPPie::slotMousePress(QMouseEvent *event)
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
    if (mKeyAxis.data()->range().size() <= 0) return;
    QPainterPath inPath, outPath;
    inPath.addEllipse(mInPixelRect);
    outPath.addEllipse(mOutPixelRect);
    if ((qFuzzyIsNull(mHoleSize) && outPath.contains(event->pos()))
        || (!qFuzzyIsNull(mHoleSize) && outPath.contains(event->pos()) && !inPath.contains(event->pos()))) {
        double x1 = keyAxis->coordToPixel(mCenterX);
        double y1 = valueAxis->coordToPixel(mRatioOffset);
        double x2 = keyAxis->coordToPixel(mCenterX+mRadius);
        double y2 = valueAxis->coordToPixel(mRatioOffset);
        double mouseAngle = getPoint2PointAngle(event->pos(),QPointF(x2,y2),QPointF(x1,y1));
        QCPPieDataMap::const_iterator it, itStart, itEnd;
        itStart = mData->cbegin();
        itEnd = mData->cend();
        double angle = 0, beginAngle = 0;
        for (it=itStart; it!=itEnd; it++) {
            angle = it.value().value*360./mSum;
            if (beginAngle < mouseAngle && mouseAngle < beginAngle+angle) {
                if (mCurSlice == it.value().key) {
                    emit sliceSelected(mCurSlice,false);
                    mCurSlice = "";
                }
                else {
                    mCurSlice = it.value().key;
                    emit sliceSelected(mCurSlice,true);
                }
            }
            beginAngle += angle;
        }
        if (mCurSlice.isEmpty())
            setName(mPieName);
        else
            setName(mPieName+":"+mCurSlice);
    }
}



