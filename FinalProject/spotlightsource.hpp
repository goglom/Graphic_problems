#pragma once

#include "lightsource.hpp"
#include "movableobject.hpp"

#include <QtMath>

class SpotLightSource;
using SpotLightSourceSPtr = std::shared_ptr<SpotLightSource>;

class SpotLightSource final : public LightSource, public MovableObject
{
public:
    float cutOff;
    float outerCutOff;
    float constFactor;
    float linFactor;
    float quadFactor;

    static SpotLightSourceSPtr construct(
            QVector3D const& position,
            QVector3D const& direction,
            QVector3D const& color = {1.f, 1.f, 1.f},
            float intensity = 1.f,
            float cutOff = qDegreesToRadians(12.f),
            float outerCutOff = qDegreesToRadians(3.f),
            float constFact  = 1.f,
            float linFact    = 0.09f,
            float quadFact   = 0.032f
            );

    void uploadToShader(std::shared_ptr<QOpenGLShaderProgram> pShader,
                        size_t index) override;

    void offsetMove(QVector3D const& offset) override;
    void moveTo(QVector3D const& postition) override;
    void setDirection(QVector3D const& direction);
    QVector3D position() const override;

private:
    QVector3D position_;
    QVector3D direction_;

    SpotLightSource(QVector3D const& position,
                    QVector3D const& direction,
                    QVector3D const& color = {1.f, 1.f, 1.f},
                    float intensity = 1.f,
                    float cutOff = qDegreesToRadians(12.f),
                    float outerCutOff = qDegreesToRadians(3.f),
                    float constFact  = 1.f,
                    float linFact    = 0.09f,
                    float quadFact   = 0.032f
                    );
};

