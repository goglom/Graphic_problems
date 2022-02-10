#pragma once

#include "renderobjectdecorator.hpp"
#include "lightsource.hpp"

class PointLightSource final : public RenderObjectDecorator, public LightSource
{

public:
    QVector3D position;
    float constFactor;
    float linFactor;
    float quadFactor;

    PointLightSource(
            RenderObjectSPtr rendered,
            QVector3D const& color = {1.f, 1.f, 1.f},
            float intensity = 1.f,
            float constFactor = 1.f,
            float linFactor =   0.045f,
            float quadFactor =  0.015f
            );

    void initialize(
            OpenGLFunctionsSPtr pFunctions,
            ShaderProgramSPtr pShaderProgram
            ) override;
    RenderObjectSPtr clone() const override;
    void render() override;
    void uploadToShader(ShaderProgramSPtr pSceneShader, size_t index) override;
    void offsetMove(QVector3D const& offset) override;
    void moveTo(QVector3D const& position) override;
};

