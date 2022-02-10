#pragma once
#include <QVector3D>
#include <QVector2D>
#include "qopengl.h"

struct Vertex
{
    QVector3D position{};
    inline static constexpr size_t positionTupleSize = sizeof(position) / sizeof(float);
    inline static constexpr GLenum positionElemTypeVal = GL_FLOAT;

    QVector3D normal{};
    inline static constexpr size_t normalTupleSize = sizeof(normal) / sizeof(float);
    inline static constexpr GLenum normalElemTypeVal = GL_FLOAT;

    QVector3D tangent{};
    inline static constexpr size_t tangentTupleSize = sizeof(tangent) / sizeof(float);
    inline static constexpr GLenum tangentElemTypeVal = GL_FLOAT;

    QVector3D biTangent{};
    inline static constexpr size_t biTangentTupleSize = sizeof(biTangent) / sizeof(float);
    inline static constexpr GLenum biTangentElemTypeVal = GL_FLOAT;

    QVector2D textCoord{};
    inline static constexpr size_t textCoordTupleSize = sizeof(textCoord) / sizeof(float);
    inline static constexpr GLenum textCoordElemTypeVal = GL_FLOAT;



    Vertex() = default;
    Vertex(QVector3D const& pos,
           QVector3D const& norm,
           QVector2D const& texture = {},
           QVector3D const& tan = {},
           QVector3D const& bitan = {}
           )
        : position(pos),
          normal(norm),
          tangent(tan),
          biTangent(bitan),
          textCoord(texture)
    {
    }
};

