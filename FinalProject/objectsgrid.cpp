#include "objectsgrid.hpp"


void ObjectsGrid::update()
{
    grid_.clear();
    grid_.resize(rows_);

    for (auto& row : grid_){
        for (size_t i = 0; i < cols_; ++i){
            row.push_back(baseObject_->clone());
        }
    }

    for (size_t rowN = 0; rowN < rows_; ++rowN){
        for (size_t colN = 0; colN < cols_; ++colN){
            grid_[rowN][colN]->offsetMove(
                        QVector3D(colN * stepLength_, 0.f, rowN * stepLength_)
                        );
        }
    }
}

ObjectsGrid::ObjectsGrid(RenderObjectSPtr baseObject,
                         float stepLenght,
                         size_t rows,
                         size_t columns
                         )
    : baseObject_(std::move(baseObject)),
      stepLength_(stepLenght),
      rows_(rows),
      cols_(columns)
{
}

void ObjectsGrid::setRows(size_t rows)
{
    rows_ = rows;
    update();
}

void ObjectsGrid::setColums(size_t columns)
{
    cols_ = columns;
    update();
}

void ObjectsGrid::setStepLength(float length)
{
    stepLength_ = length;
    update();
}

void ObjectsGrid::render()
{
    for (auto& row: grid_){
        for (auto& elem : row){
            elem->render();
        }
    }
}

void ObjectsGrid::initialize(OpenGLFunctionsSPtr pFunctions, ShaderProgramSPtr pShaderProgram)
{
    baseObject_->initialize(std::move(pFunctions), std::move(pShaderProgram));
    update();
}

QVector3D ObjectsGrid::position() const
{
    return {};
}

ObjectsGrid::RenderObjectSPtr ObjectsGrid::clone() const
{
    return std::make_shared<ObjectsGrid>(*this);
}

QOpenGLShaderProgram& ObjectsGrid::shader()
{
    return baseObject_->shader();
}

QOpenGLFunctions& ObjectsGrid::GLFunctions()
{
    return baseObject_->GLFunctions();
}
