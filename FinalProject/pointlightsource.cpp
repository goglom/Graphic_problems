#include "pointlightsource.hpp"

PointLightSource::PointLightSource(
        RenderObjectSPtr rendered,
        QVector3D const& color,
        float intensity,
        float _constFactor,
        float _linFactor,
        float _quadFactor
        )
    : RenderObjectDecorator(std::move(rendered)),
      LightSource(color, intensity),
      position(parent_->position()),
      constFactor(_constFactor),
      linFactor(_linFactor),
      quadFactor(_quadFactor)
{
}

void PointLightSource::initialize(OpenGLFunctionsSPtr pFunctions, ShaderProgramSPtr pShaderProgram)
{
    RenderObjectDecorator::initialize(std::move(pFunctions), std::move(pShaderProgram));
}

RenderObjectSPtr PointLightSource::clone() const
{
    return std::make_shared<PointLightSource>(*this);
}

void PointLightSource::render()
{
    shader().bind();
    shader().setUniformValue("lightColor", color);
    RenderObjectDecorator::render();
}

void PointLightSource::uploadToShader(ShaderProgramSPtr pSceneShader, size_t index)
{
    pSceneShader->bind();
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].color").c_str(), color);
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].position").c_str(), position);
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].intensity").c_str(), intensity);
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].constFactor").c_str(), constFactor);
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].linFactor").c_str(), linFactor);
    pSceneShader->setUniformValue(("pointLights[" + std::to_string(index) + "].quadFactor").c_str(), quadFactor);
}

void PointLightSource::offsetMove(const QVector3D& offset)
{
    RenderObjectDecorator::offsetMove(offset);
    position = RenderObjectDecorator::position();
}

void PointLightSource::moveTo(const QVector3D& _position)
{
    position = _position;
    RenderObjectDecorator::moveTo(_position);
}
