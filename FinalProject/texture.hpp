#pragma once
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <memory>
#include <atomic>
#include <list>

class Texture : public QOpenGLTexture
{
public:
    enum class Type{
        ColorMap,
        NormalMap,
    };

    Type type;

    Texture(QImage const& image, Type type);
};

