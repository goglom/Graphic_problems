#include "meshobject.hpp"


MeshObject::MeshObject(
        MeshSPtr mesh,
        const QMatrix4x4& modelMatrix
        )
    : pMesh_(std::move(mesh)),
      modelMatrix_(modelMatrix)
{

}

MeshObjectSPtr MeshObject::construct(
        MeshSPtr mesh,
        const QMatrix4x4& modelMatrix
        )
{
    return MeshObjectSPtr(new MeshObject(
                              std::move(mesh),
                              modelMatrix)
                          );
}

RenderObjectSPtr MeshObject::clone() const
{
    return MeshObjectSPtr(new MeshObject(*this));
}

void MeshObject::initialize(OpenGLFunctionsSPtr pFunctions,
                            ShaderProgramSPtr pShaderProgram)
{
    Q_ASSERT(pShaderProgram);
    Q_ASSERT(pFunctions);
    pFunctions_ = std::move(pFunctions);
    pShader_ = std::move(pShaderProgram);

    if (VAO_->isCreated())
        return;

    QOpenGLVertexArrayObject::Binder binder(VAO_.get());
    VBO_.create();
    VBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO_.bind();
    VBO_.allocate(pMesh_->vertices.data(),
                  pMesh_->vertices.size() * sizeof(Mesh::vertexType)
                  );

    pShader_->bind();

    // Vertex position
    pShader_->enableAttributeArray(positionLocation);
    pShader_->setAttributeBuffer(
                positionLocation, Mesh::vertexType::positionElemTypeVal, offsetof(Mesh::vertexType, position),
                Mesh::vertexType::positionTupleSize, sizeof(Mesh::vertexType)
                );
    // Vertex normal
    pShader_->enableAttributeArray(normalLocation);

    pShader_->setAttributeBuffer(
                normalLocation, Mesh::vertexType::normalElemTypeVal, offsetof(Mesh::vertexType, normal),
                Mesh::vertexType::normalTupleSize, sizeof(Mesh::vertexType)
                );
    // Vertex Texture Coordinate
    // If there is no texture in MeshObject initialize don't load texture coordinates
    pShader_->enableAttributeArray(textCoordLocation);
    pShader_->setAttributeBuffer(
                textCoordLocation, Mesh::vertexType::textCoordElemTypeVal, offsetof(Mesh::vertexType, textCoord),
                Mesh::vertexType::textCoordTupleSize, sizeof(Mesh::vertexType)
                );

    IBO_.create();
    IBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    IBO_.bind();
    IBO_.allocate(pMesh_->indices.data(),
                  pMesh_->indices.size() * sizeof(Mesh::indexType)
                  );
}

void MeshObject::render()
{
    if (modelChanged_){
        normModelMatrix_ = modelMatrix_.inverted().transposed();
        modelChanged_ = false;
    }
    shader().bind();
    shader().setUniformValue("model", modelMatrix_);
    shader().setUniformValue("normModel", normModelMatrix_);
    GLint colorMapCount = 0;
    GLint normalMapCount = 0;

    for (GLuint i = 0; i < textures.size(); ++i){
        std::string textureName;// = TextureTypeToName.at(textures[i]->type) + std::to_string(i);
        auto& [pTexture, enabled] = textures[i];

        if (pTexture->type == Texture::Type::ColorMap){
            textureName.append("ColorMap[" + std::to_string(colorMapCount) + "]");
            shader().setUniformValue((textureName + ".map").c_str(), i);
            shader().setUniformValue((textureName + ".enabled").c_str(), enabled);
            ++colorMapCount;
        }
        else if (pTexture->type == Texture::Type::NormalMap){
            textureName.append("NormalMap" + std::to_string(colorMapCount));
            ++normalMapCount;
            shader().setUniformValue((textureName + "Enabled").c_str(), enabled);
            shader().setUniformValue(textureName.c_str(), i);
        }

        GLFunctions().glActiveTexture(GL_TEXTURE0 + i);
        pTexture->bind();
    }
    shader().setUniformValue("ColorMapCount", colorMapCount);

    QOpenGLVertexArrayObject::Binder binder(VAO_.get());
    pFunctions_->glDrawElements(GL_TRIANGLES, pMesh_->indices.size(), Mesh::IndexTypeVal, nullptr);
}

QMatrix4x4& MeshObject::getModel()
{
    modelChanged_ = true;
    return modelMatrix_;
}

QMatrix4x4 const& MeshObject::getModel() const
{
    return modelMatrix_;
}

void MeshObject::offsetMove(const QVector3D& offset)
{
    getModel().translate(offset);
}

void MeshObject::moveTo(const QVector3D& position)
{
    getModel().setColumn(3, {position, 1.f});
}

void MeshObject::rotate(const QQuaternion& rotation)
{
    QMatrix4x4 rot;
    rot.rotate(rotation);
    getModel() = rot * getModel();
}

QVector3D MeshObject::position() const
{
    return QVector3D{getModel().column(3)};
}

QOpenGLShaderProgram& MeshObject::shader()
{
    Q_ASSERT(pShader_);
    return *pShader_;
}

QOpenGLFunctions& MeshObject::GLFunctions()
{
    Q_ASSERT(pFunctions_);
    return *pFunctions_;
}
