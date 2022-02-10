#include "scene.hpp"

#include "glcolor.hpp"



void Scene::initialize(OpenGLFunctionsSPtr pFunctions, Scene::ShaderProgramSptr object, Scene::ShaderProgramSptr lightSrc)
{
    pFunctions_ = std::move(pFunctions);
    objectShader_ = std::move(object);
    lightSourceShader_ = std::move(lightSrc);

    objectShader_->bind();
    objectShader_->setUniformValue("dirLightsCount", GLint(0));
    objectShader_->setUniformValue("pointLightsCount", GLint(0));

}

void Scene::setAmbientColor(const QColor& color)
{
    ambientColor_ = getGlColor(color);
}

void Scene::setAmbientColor(const QVector3D& color)
{
    ambientColor_ = color;
}

const QVector3D& Scene::getAmbientColor() const
{
    return ambientColor_;
}

void Scene::addPointLightSource(Scene::PointLightSourseSPtr light)
{
    light->initialize(pFunctions_, lightSourceShader_);
    objects.push_back(std::static_pointer_cast<RenderObject>(light));
    pointLightSources.push_back(std::move(light));

    objectShader_->bind();
    objectShader_->setUniformValue("pointLightsCount", GLint(pointLightSources.size()));
}

void Scene::addDirectlyLightSource(Scene::DirectlyLightSourseSPtr light)
{
    dirLightSources.push_back(std::move(light));

    objectShader_->bind();
    objectShader_->setUniformValue("dirLightsCount", GLint(dirLightSources.size()));
}

void Scene::addSpotLightSource(Scene::SpotLightSourseSPtr light)
{
    spotLightsSources.push_back(std::move(light));

    objectShader_->bind();
    objectShader_->setUniformValue("spotLightsCount", GLint(spotLightsSources.size()));
}

void Scene::addRenderObject(Scene::RenderObjectSPtr object)
{
    object->initialize(pFunctions_, objectShader_);
    objects.push_back(std::move(object));
}

void Scene::renderAll()
{
    objectShader_->bind();
    objectShader_->setUniformValue("ambientColor", ambientColor_);
    objectShader_->setUniformValue("dirLightsCount",    static_cast<GLint>(dirLightSources.size()));
    objectShader_->setUniformValue("pointLightsCount",  static_cast<GLint>(pointLightSources.size()));
    objectShader_->setUniformValue("spotLightsCount",   static_cast<GLint>(spotLightsSources.size()));

    for (size_t i = 0; i < pointLightSources.size(); ++i){
        pointLightSources[i]->uploadToShader(objectShader_, i);
    }
    for (size_t i = 0; i < dirLightSources.size(); ++i){
        dirLightSources[i]->uploadToShader(objectShader_, i);
    }
    for (size_t i = 0; i < spotLightsSources.size(); ++i){
        spotLightsSources[i]->uploadToShader(objectShader_, i);
    }
    for (auto& obj : objects){
        obj->render();
    }
}

void Scene::clean()
{
    objects.clear();
    dirLightSources.clear();
    pointLightSources.clear();
    spotLightsSources.clear();
}
