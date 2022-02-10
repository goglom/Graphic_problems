#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "dialog.hpp"

QColor getQColor(QVector3D const& glColor){
    QColor result;
    result.setRedF(glColor.x());
    result.setGreenF(glColor.y());
    result.setBlueF(glColor.z());
    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dialog(new Dialog(this))
{
    ui->setupUi(this);

    QObject::connect(dialog, &Dialog::ObjectMaterialChanged,
                     ui->Scene, &SceneWidget::setObjectMaterial);
    QObject::connect(dialog, &Dialog::AmbientColorChanged,
                     ui->Scene, &SceneWidget::setAmbientColor);
    QObject::connect(ui->Scene, &SceneWidget::newMaterial,
                     dialog, &Dialog::addMaterial);
    QObject::connect(dialog, &Dialog::DirLightIntensityChanged,
                     ui->Scene, &SceneWidget::setDirInt);
    QObject::connect(dialog, &Dialog::DirLightColorChanged,
                     ui->Scene, &SceneWidget::setDirColor);
    QObject::connect(dialog, &Dialog::PointLightColorChanged,
                     ui->Scene, &SceneWidget::setPointColor);
    QObject::connect(dialog, &Dialog::SpotLightColorChanged,
                     ui->Scene, &SceneWidget::setSpotColor);
    QObject::connect(dialog, &Dialog::DirLightDirectionChanged,
                     ui->Scene, &SceneWidget::setDirDirection);
    QObject::connect(dialog, &Dialog::PointLightIntensityChanged,
                     ui->Scene, &SceneWidget::setPointInt);
    QObject::connect(dialog, &Dialog::PointLightConstChanged,
                     ui->Scene, &SceneWidget::setPointConst);
    QObject::connect(dialog, &Dialog::PointLightLinChanged,
                     ui->Scene, &SceneWidget::setPointLin);
    QObject::connect(dialog, &Dialog::PointLightQuadChanged,
                     ui->Scene, &SceneWidget::setPointQuad);
    QObject::connect(dialog, &Dialog::SpotLightIntensityChanged,
                     ui->Scene, &SceneWidget::setSpotInt);
    QObject::connect(dialog, &Dialog::SpotLightConstChanged,
                     ui->Scene, &SceneWidget::setSpotConst);
    QObject::connect(dialog, &Dialog::SpotLightLinChanged,
                     ui->Scene, &SceneWidget::setSpotLin);
    QObject::connect(dialog, &Dialog::SpotLightQuadChanged,
                     ui->Scene, &SceneWidget::setSpotQuad);
    QObject::connect(dialog, &Dialog::CutOffChanged,
                     ui->Scene, &SceneWidget::setCutOff);
    QObject::connect(dialog, &Dialog::OuterCutOffChanged,
                     ui->Scene, &SceneWidget::setOuterCutOff);

    dialog->AmbientColor = getQColor(ui->Scene->ambientColor);
    dialog->DirLightColor = getQColor(ui->Scene->dirLightColor);
    dialog->SpotLightColor = getQColor(ui->Scene->spotLightColor);
    dialog->PointLightColor = getQColor(ui->Scene->pointLightColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SceneOptionsButton_clicked()
{
    dialog->show();
}
