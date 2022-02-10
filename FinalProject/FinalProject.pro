TEMPLATE = app
CONFIG += c++17

QT += core gui widgets

SOURCES += \
        main.cpp \
        treebranch.cpp

HEADERS += \
    cameraview.hpp \
    dialog.hpp \
    direcltylight.hpp \
    glcolor.hpp \
    keyboard.hpp \
    lightsource.hpp \
    mainwindow.hpp \
    material.hpp \
    materialfactory.hpp \
    materialobject.hpp \
    mesh.hpp \
    meshcollectionobject.hpp \
    meshfactory.hpp \
    meshobject.hpp \
    movableobject.hpp \
    objectsgrid.hpp \
    pointlightsource.hpp \
    renderobject.hpp \
    renderobjectdecorator.hpp \
    scene.hpp \
    scenewidget.hpp \
    spotlightsource.hpp \
    texture.hpp \
    treebranch.hpp \
    vertex.hpp
