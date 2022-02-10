#pragma once

#include "renderobject.hpp"
#include "meshobject.hpp"

#include <memory>
#include <vector>

class MeshCollectionObject;
using MeshCollectionObjectSPtr = std::shared_ptr<MeshCollectionObject>;

class MeshCollectionObject : public RenderObject
{
public:
    static MeshCollectionObjectSPtr construct(std::vector<MeshObjectSPtr> meshObjects);

    virtual QOpenGLShaderProgram& shader() override;
    virtual QOpenGLFunctions& GLFunctions() override;

    virtual void render() override;
    virtual void initialize(
            OpenGLFunctionsSPtr pOpenGLFunctions,
            ShaderProgramSPtr pShaderProgram
            ) override;
    virtual RenderObjectSPtr clone() const override;
    QVector3D position() const override;

    std::vector<MeshObjectSPtr> meshes;

private:
    MeshCollectionObject(std::vector<MeshObjectSPtr> meshObjects);

    QMatrix4x4 model_;
    OpenGLFunctionsSPtr pFunctions_;
    ShaderProgramSPtr pShader_;
};
