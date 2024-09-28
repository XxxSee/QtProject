#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QVector2D>
#include <QVector3D>
#include <QPainter>

class Renderer : public QObject
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = nullptr);
    void addObj(QVector3D obj);
    void addObj(int x, int y, int z);
    void drawObj(QPainter *painter);
    QVector3D &camera();
    float &dirX();
    float &dirY();
private:
    QVector2D get3dto2d(const QVector3D &obj);
    QVector3D getdirX(QVector3D obj,float dir);
    QVector3D getdirY(QVector3D obj,float dir);
private:
    QVector3D mCamera;
    float mDis;
    float mDirX;
    float mDirY;
    QList<QVector3D> mObjs;
};

#endif // RENDERER_H
