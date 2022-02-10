#pragma once
#include <QVector3D>
#include <QColor>

inline QVector3D getGlColor(QColor const& color){
    QVector3D result;
    result.setX(color.redF());
    result.setY(color.greenF());
    result.setZ(color.blueF());

    return result;
}
