#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <cmath>

class CameraView
{
    QMatrix4x4 view_{};
    QMatrix4x4 projection_{};
    QMatrix4x4 projView_{};

    QVector3D eye_pos_;
    QVector3D up_;
    QVector3D front_;
    QVector3D right_;
    QVector3D worldUp_;

    float aspectRatio_;
    float zNear_;
    float zFar_;
    float fov_;

    float yaw_ =  - M_PI / 2;
    float pitch_ = 0.f;
    float pitchLB = -M_PI / 2.2f;
    float pitchUB = M_PI / 2.2f;

    void updateVectors();

public:
    inline static constexpr bool constrainPitch = true;

    enum class directions{
        forward,
        backward,
        left,
        right,
        up,
        down,
    };

    CameraView(
            QVector3D const& eye_pos = {},
            QVector3D const& front = {0.f, 0.f, -1.f},
            QVector3D const& up = {0.f, 1.f, 0.f}
            );
    QMatrix4x4 const& getViewMatrix();
    QMatrix4x4 const& getProjectionMatrix();
    QMatrix4x4 const& getProjViewMatrix();
    const QVector3D& cameraPosition() const;


    void setProjection(float fov, float aspectRatio, float zNear, float zFar);
    void setFOV(float fov);
    void setAspectRatio(float ratio);
    void setZNear(float zNear);
    void setZFar(float zFar);

    void move(QVector3D const& offset);
    void move(directions direction, float length);
    void rotate(float yaw, float pitch);

};

