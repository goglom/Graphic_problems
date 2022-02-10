#include "meshcollectionobject.hpp"

MeshCollectionObjectSPtr MeshCollectionObject::construct(std::vector<MeshObjectSPtr> meshObjects)
{
    return MeshCollectionObjectSPtr{new MeshCollectionObject(std::move(meshObjects))};
}

QOpenGLShaderProgram& MeshCollectionObject::shader()
{
    return *pShader_;
}

QOpenGLFunctions& MeshCollectionObject::GLFunctions()
{
    return *pFunctions_;
}

void MeshCollectionObject::render()
{
    for (auto& mesh : meshes){
        mesh->render();
    }
}

void MeshCollectionObject::initialize(OpenGLFunctionsSPtr pOpenGLFunctions, ShaderProgramSPtr pShaderProgram)
{
    pFunctions_ = std::move(pOpenGLFunctions);
    pShader_ = std::move(pShaderProgram);

    for (auto& mesh : meshes){
        mesh->initialize(pFunctions_, pShader_);
    }
}

RenderObjectSPtr MeshCollectionObject::clone() const
{
    throw std::runtime_error("<RenderObjectSPtr Model3DObject::clone() const>: not implemented!");
}

QVector3D MeshCollectionObject::position() const
{
    return QVector3D{model_.column(3)};
}

MeshCollectionObject::MeshCollectionObject(std::vector<MeshObjectSPtr> meshObjects)
    : meshes(std::move(meshObjects))
{

}
