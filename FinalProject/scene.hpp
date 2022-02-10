#pragma once

#include "lightsource.hpp"
#include "meshobject.hpp"
#include "pointlightsource.hpp"
#include "direcltylight.hpp"
#include "spotlightsource.hpp"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <memory>
#include <vector>

class Scene
{
    using LightSourseSPtr = std::shared_ptr<LightSource>;
    using PointLightSourseSPtr = std::shared_ptr<PointLightSource>;
    using DirectlyLightSourseSPtr = std::shared_ptr<DirecltyLightSource>;
    using SpotLightSourseSPtr = std::shared_ptr<SpotLightSource>;
    using RenderObjectSPtr = std::shared_ptr<RenderObject>;
    using ShaderProgramSptr =  std::shared_ptr<QOpenGLShaderProgram>;


    OpenGLFunctionsSPtr pFunctions_;
    ShaderProgramSptr objectShader_;
    ShaderProgramSptr lightSourceShader_;
    QVector3D ambientColor_{};

public:
    std::vector<PointLightSourseSPtr> pointLightSources;
    std::vector<DirectlyLightSourseSPtr> dirLightSources;
    std::vector<SpotLightSourseSPtr> spotLightsSources;
    std::vector<RenderObjectSPtr> objects;


    void initialize(
            OpenGLFunctionsSPtr pFunctions,
            ShaderProgramSptr object,
            ShaderProgramSptr lightSrc
            );

    void setAmbientColor(QColor const& color);
    void setAmbientColor(QVector3D const& color);
    QVector3D const& getAmbientColor() const;

    void addPointLightSource(PointLightSourseSPtr light);
    void addDirectlyLightSource(DirectlyLightSourseSPtr light);
    void addSpotLightSource(SpotLightSourseSPtr light);
    void addRenderObject(RenderObjectSPtr object);

    void renderAll();
    void clean();
};
