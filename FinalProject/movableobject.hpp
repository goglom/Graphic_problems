#pragma once

#include <QVector3D>
#include <QQuaternion>

class MovableObject
{   
public:
    virtual void offsetMove(QVector3D const&){};
    virtual void moveTo(QVector3D const&){};
    virtual void rotate(QQuaternion const&){};
    virtual QVector3D position() const = 0;
    virtual ~MovableObject() = default;
};
