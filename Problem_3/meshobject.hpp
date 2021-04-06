﻿#pragma once

#include "mesh.hpp"
#include "renderobject.hpp"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <memory>

class MeshObject;
using MeshObjectSPtr = std::shared_ptr<MeshObject>;
using MeshSPtr = std::shared_ptr<Mesh>;

class MeshObject final: public RenderObject
{
    using VaoSPtr = std::shared_ptr<QOpenGLVertexArrayObject>;    
    VaoSPtr VAO_ = std::make_shared<QOpenGLVertexArrayObject>();
    QOpenGLBuffer VBO_{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer IBO_{QOpenGLBuffer::IndexBuffer};

    MeshSPtr pMesh_;
    QMatrix4x4 modelMatrix_;
    QMatrix4x4 normModelMatrix_;

    bool modelChanged_ = true;
    inline static constexpr int positionLocation = 0;
    inline static constexpr int normalLocation = 1;
    inline static constexpr int textCoordLocation = 2;

    MeshObject(
            MeshSPtr mesh,
            QMatrix4x4 const& modelMatrix
            );
protected:
    ShaderProgramSPtr pShader_;
    OpenGLFunctionsSPtr pFunctions_;

public:    
    static MeshObjectSPtr construct(
            MeshSPtr mesh,
            QMatrix4x4 const& modelMatrix = {}
            );

    RenderObjectSPtr clone() const override;
    void initialize(OpenGLFunctionsSPtr pFunctions,
                    ShaderProgramSPtr pShaderProgram
                    ) override;
    void render() override;

    QMatrix4x4& getModel();
    QMatrix4x4 const& getModel() const;

    void offsetMove(QVector3D const& offset) override;
    void moveTo(QVector3D const& position) override;

    void rotate(QQuaternion const& rotation) override;

    QVector3D position() const override;
    QOpenGLShaderProgram& shader()  override;
    QOpenGLFunctions& GLFunctions() override;
};

