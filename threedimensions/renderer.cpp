#include "renderer.h"

Renderer::Renderer(QObject *parent) : QObject(parent)
{
    mCamera = QVector3D(1,1,1);
    mDis = 200;
    mDirX = 0;
    mDirY = 0;
}

void Renderer::addObj(QVector3D obj)
{
    mObjs << obj;
}

void Renderer::addObj(int x, int y, int z)
{
    mObjs << QVector3D(x,y,z);
}

QVector2D Renderer::get3dto2d(const QVector3D &obj)
{
    QVector3D a;
    a.setX(obj.x() - mCamera.x());
    a.setY(obj.y() - mCamera.y());
    a.setZ(obj.z() - mCamera.z());
    QVector2D b;
    b.setX(a.x()*mDis/a.z());
    b.setY(a.y()*mDis/a.z());
    return b;
}

QVector3D Renderer::getdirX(QVector3D obj, float dir)
{
    dir = -dir;
    QVector3D objr = obj;
    objr.setX(obj.z()*sin(dir)+obj.x()*cos(dir));
    objr.setZ(obj.z()*cos(dir)-obj.x()*sin(dir));
    return objr;
}

QVector3D Renderer::getdirY(QVector3D obj, float dir)
{
    dir = -dir;
    QVector3D objr = obj;
    objr.setY(obj.z()*sin(dir)+obj.y()*cos(dir));
    objr.setZ(obj.z()*cos(dir)-obj.y()*sin(dir));
    return objr;
}

void Renderer::drawObj(QPainter *painter)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    for(int i{0}; i<mObjs.size(); i++) {
        QVector3D obj = getdirY(mObjs[i],mDirY);
        obj = getdirX(obj,mDirX);
        QVector2D p = get3dto2d(obj);
        painter->drawRect(p.x()-5,p.y()-5,10,10);
    }
    for(int i{0}; i<mObjs.size(); i++) {
        for(int j{0}; j<mObjs.size(); j++) {
            if(abs(mObjs[i].x()-mObjs[j].x())+abs(mObjs[i].y()-mObjs[j].y())+abs(mObjs[i].z()-mObjs[j].z())==100) {
                QVector3D obj1 = getdirY(mObjs[i],mDirY);
                obj1 =getdirX(obj1,mDirX);
                QVector2D p1 = get3dto2d(obj1);
                QVector3D obj2 = getdirY(mObjs[j],mDirY);
                obj2 =getdirX(obj2,mDirX);
                QVector2D p2 = get3dto2d(obj2);
                painter->drawLine(p1.x(),p1.y(),p2.x(),p2.y());
            }
        }
    }
    painter->restore();
}

QVector3D &Renderer::camera()
{
    return mCamera;
}

float &Renderer::dirX()
{
    return mDirX;
}

float &Renderer::dirY()
{
    return mDirY;
}
