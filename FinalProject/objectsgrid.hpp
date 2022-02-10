#pragma once

#include "renderobject.hpp"

#include <vector>
#include <memory>

class ObjectsGrid final : public RenderObject
{
    using RenderObjectSPtr = std::shared_ptr<RenderObject>;

    RenderObjectSPtr baseObject_;
    std::vector<std::vector<RenderObjectSPtr>> grid_;
    float stepLength_;
    size_t rows_;
    size_t cols_;

    void update();

public:
    ObjectsGrid(RenderObjectSPtr baseObject,
                float stepLenght,
                size_t rows = 2,
                size_t columns = 2
                );
    void setRows(size_t rows);
    void setColums(size_t columns);
    void setStepLength(float length);

    void render() override;
    void initialize(
            OpenGLFunctionsSPtr pFunctions,
            ShaderProgramSPtr pShaderProgram
            ) override;
    QVector3D position() const override;
    RenderObjectSPtr clone() const override;
    QOpenGLShaderProgram& shader() override;
    QOpenGLFunctions& GLFunctions() override;
};

