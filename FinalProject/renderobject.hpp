#pragma once

#include "movableobject.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <memory>

class RenderObject;
using RenderObjectSPtr = std::shared_ptr<RenderObject>;
using ShaderProgramSPtr = std::shared_ptr<QOpenGLShaderProgram>;
using OpenGLFunctionsSPtr = std::shared_ptr<QOpenGLFunctions>;

class RenderObject : public MovableObject
{
public:
    virtual QOpenGLShaderProgram& shader() = 0;
    virtual QOpenGLFunctions& GLFunctions() = 0;

    virtual void render() = 0;
    virtual void initialize(
            OpenGLFunctionsSPtr pOpenGLFunctions,
            ShaderProgramSPtr pShaderProgram
            ) = 0;
    virtual RenderObjectSPtr clone() const = 0;
};
