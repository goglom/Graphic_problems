/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "cubewidget.hpp"

#include <QMouseEvent>
#include <iostream>
#include <cmath>


CubeWidget::CubeWidget(QWidget* parent)
     : QOpenGLWidget(parent),
       scene(*this),
       keyStates(maxKeyCode, false)
{

}

CubeWidget::~CubeWidget()
{
    makeCurrent();
}

void CubeWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton){
        mouseLastPosition = QVector2D(e->pos());
    }
}

void CubeWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton){
        QVector2D diff = QVector2D(event->pos()) - mouseLastPosition;
        diff *= 0.001f;
        camera.rotate(diff.x(), diff.y());
    }
    mouseLastPosition = QVector2D(event->pos());
}

void CubeWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() < keyStates.size()){
        keyStates[event->key()] = true;
    }
}

void CubeWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() < keyStates.size()){
        keyStates[event->key()] = false;
    }
}

void CubeWidget::moveProcess(float deltaTime)
{
    float length = cameraSpeed * deltaTime;

    if (keyStates.at(Qt::Key_W)){
        camera.move(CameraView::directions::forward, length);
    }
    if (keyStates.at(Qt::Key_S)){
        camera.move(CameraView::directions::backward, length);
    }
    if (keyStates.at(Qt::Key_D)){
        camera.move(CameraView::directions::right, length);
    }
    if (keyStates.at(Qt::Key_A)){
        camera.move(CameraView::directions::left, length);
    }
    if (keyStates.at(Qt::Key_Space)){
        camera.move(CameraView::directions::up, length);
    }
    if (keyStates.at(Qt::Key_Z)){
        camera.move(CameraView::directions::down, length);
    }
}

void CubeWidget::wheelEvent(QWheelEvent* ev)
{
    fov += ev->angleDelta().y() / 60.f;
    fov = std::clamp(fov, 10.f, std::numeric_limits<float>::infinity());
}

void CubeWidget::timerEvent(QTimerEvent*)
{
    moveProcess(deltaTimeMsec / 1000.f); // deltaTime in sec
    update();
}

QSize CubeWidget::minimumSizeHint() const
{
    return {640, 480};
}

void CubeWidget::initShaders()
{
    Q_ASSERT(objShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GuroSceneObject.vert"));
    Q_ASSERT(objShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GuroSceneObject.frag"));
    Q_ASSERT(objShader->link());

    Q_ASSERT(lightShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LightSource.vert"));
    Q_ASSERT(lightShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GuroSceneObject.frag"));
    Q_ASSERT(lightShader->link());
}

void CubeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    initShaders();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    scene.initialize(objShader, lightShader);

    QMatrix4x4 model;
    model.rotate(30, 0.f, 1.f);

    scene.addRenderObject(std::make_shared<SceneObject>(
                              MeshFactory::makeCube({1, 1, 1}, 50),
                   model,
                   MaterialFactory::makeGlod()
                   )
    );

    model.setToIdentity();
    model.translate(0.f, 0.f, -5.f);
    model.rotate(30, 0.f, 1.f);
    model.rotate(30, 1.f, 0.f);
    scene.addRenderObject(std::make_shared<SceneObject>(
                              MeshFactory::makeCube({2.f, 2.f, 2.f}, 50),
                              model,
                              MaterialFactory::makeCyanPlastic()
                              )
                          );

//    scene.addDirectlyLightSource(std::make_shared<DirecltyLight>(
//                        QVector3D(1.f, 1.f, 1.f),
//                        0.5f,
//                        QVector3D(0.f, -1.f, 0.f)
//                        )
//    );
    model.setToIdentity();
    model.translate(0.f, 0.f, 2.f);

    scene.addPointLightSource(
                std::make_shared<PointLightSource>(
                    MeshFactory::makeCube({0.1, 0.1, 0.1f}),
                    model,
                    QVector3D{1.f, 1.f, 1.f},
                    1.f,
                    1.f,
                    0.1f,
                    0.5)
                );
    model.setToIdentity();
    model.translate(0.f, 2.f, -3.f);

//    scene.addPointLightSource(
//                std::make_shared<PointLightSource>(
//                    MeshFactory::makeCube({0.1, 0.1, 0.1f}),
//                    model,
//                    QVector3D{1.f, 0.1f, 0.1f},
//                    8.f)
//                );

//    scene.addDirectlyLightSource(std::make_shared<DirecltyLight>(
//                        QColor(255, 255, 255),
//                        0.1f,
//                        QVector3D(1.f, 0.0f, -1.f)
//                        )
//    );

    camera.setZNear(zNear);
    camera.setZFar(zFar);
    timer.start(deltaTimeMsec, this);
}



void CubeWidget::resizeGL(int w, int h)
{
    camera.setAspectRatio(float(w) / float(h ? h : 1));
}

void CubeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.setFOV(fov);

    objShader->bind();
    objShader->setUniformValue("projView",camera.getProjViewMatrix());
    objShader->setUniformValue("cameraPos", camera.cameraPosition());

    lightShader->bind();
    lightShader->setUniformValue("projView",camera.getProjViewMatrix());
    lightShader->setUniformValue("cameraPos", camera.cameraPosition());

    scene.renderAll();
}

