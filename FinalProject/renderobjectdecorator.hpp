#pragma once

#include "renderobject.hpp"

class RenderObjectDecorator : public RenderObject
{
protected:
    RenderObjectSPtr parent_;

public:
    RenderObjectDecorator(RenderObjectSPtr parent);
    virtual void render() override;
    RenderObjectSPtr clone() const override;
    virtual void initialize(OpenGLFunctionsSPtr pFunctions,
                            ShaderProgramSPtr pShaderProgram
                            ) override;
    QVector3D position() const override;
    void offsetMove(QVector3D const& offset) override;
    void moveTo(QVector3D const& position) override;
    QOpenGLShaderProgram& shader() override;
    QOpenGLFunctions& GLFunctions() override;
    RenderObject& getParent();

};

inline RenderObjectDecorator::RenderObjectDecorator(RenderObjectSPtr parent)
    : parent_(std::move(parent))
{
}

inline void RenderObjectDecorator::render()
{
    parent_->render();
}

inline RenderObjectSPtr RenderObjectDecorator::clone() const
{
    return std::make_shared<RenderObjectDecorator>(parent_->clone());
}

inline void RenderObjectDecorator::initialize(OpenGLFunctionsSPtr pFunctions, ShaderProgramSPtr pShaderProgram)
{
    parent_->initialize(std::move(pFunctions), std::move(pShaderProgram));
}

inline QVector3D RenderObjectDecorator::position() const
{
    return parent_->position();
}

inline void RenderObjectDecorator::offsetMove(const QVector3D& offset)
{
    parent_->offsetMove(offset);
}

inline void RenderObjectDecorator::moveTo(const QVector3D& position)
{
    parent_->moveTo(position);
}

inline QOpenGLShaderProgram& RenderObjectDecorator::shader()
{
    return parent_->shader();
}

inline QOpenGLFunctions& RenderObjectDecorator::GLFunctions()
{
    return parent_->GLFunctions();
}

inline RenderObject& RenderObjectDecorator::getParent()
{
    return *parent_;
}

