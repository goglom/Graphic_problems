#pragma once

#include "lightsource.hpp"

class DirecltyLightSource final : public LightSource
{
public:
    QVector3D direction;

    DirecltyLightSource(QVector3D const& color,
                  float intensity,
                  QVector3D const& direction
                  );
    void uploadToShader(std::shared_ptr<QOpenGLShaderProgram> shader,
                                size_t index) override;
};

