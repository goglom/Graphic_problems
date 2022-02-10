#include "dialog.hpp"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    colorDialog(new QColorDialog(this))
{
    ui->setupUi(this);

    QObject::connect(colorDialog, &QColorDialog::currentColorChanged,
                     this, &Dialog::setLightColor);

    dirLightDirection.setX(ui->DirLightXSpinBox->value());
    dirLightDirection.setY(ui->DirLightYSpinBox->value());
    dirLightDirection.setZ(ui->DirLightZSpinBox->value());
    emit DirLightDirectionChanged(dirLightDirection);
}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::addMaterial(const QString& name)
{
    ui->MaterialComboBox->addItem(name);
}

void Dialog::setLightColor(const QColor& color)
{
    switch (lightSource) {
    case LightSources::Ambient :
        AmbientColor = color;
        emit AmbientColorChanged(color);
        break;
    case LightSources::Direct :
        DirLightColor = color;
        emit DirLightColorChanged(color);
        break;
    case LightSources::Point:
        PointLightColor = color;
        emit PointLightColorChanged(color);
        break;
    case LightSources::Spot:
        SpotLightColor = color;
        emit SpotLightColorChanged(color);
        break;
    }
}

void Dialog::on_MaterialComboBox_currentIndexChanged(int index)
{
    emit ObjectMaterialChanged(index);
}

void Dialog::on_DirLightXSpinBox_valueChanged(double arg1)
{
    dirLightDirection.setX(arg1);
    emit DirLightDirectionChanged(dirLightDirection);
}

void Dialog::on_DirLightYSpinBox_valueChanged(double arg1)
{
    dirLightDirection.setY(arg1);
    emit DirLightDirectionChanged(dirLightDirection);
}

void Dialog::on_DirLightZSpinBox_valueChanged(double arg1)
{
    dirLightDirection.setZ(arg1);
    emit DirLightDirectionChanged(dirLightDirection);
}

void Dialog::on_DirLightColorButton_clicked()
{
    lightSource = LightSources::Direct;
    colorDialog->setCurrentColor(DirLightColor);
    colorDialog->show();
}

void Dialog::on_PointLightColorButton_clicked()
{
    lightSource = LightSources::Point;
    colorDialog->setCurrentColor(PointLightColor);
    colorDialog->show();
}

void Dialog::on_PointLightConstSpinBox_valueChanged(double arg1)
{
    emit PointLightConstChanged(arg1);
}

void Dialog::on_PointLightLinSpinBox_valueChanged(double arg1)
{
    emit PointLightLinChanged(arg1);
}

void Dialog::on_PointLightQuadSpinBox_valueChanged(double arg1)
{
    emit PointLightQuadChanged(arg1);
}

void Dialog::on_SpotLightConstSpinBox_valueChanged(double arg1)
{
    emit SpotLightConstChanged(arg1);
}

void Dialog::on_SpotLightLinSpinBox_valueChanged(double arg1)
{
    emit SpotLightLinChanged(arg1);
}

void Dialog::on_SpotLightQuadSpinBox_valueChanged(double arg1)
{
    emit SpotLightQuadChanged(arg1);
}

void Dialog::on_CutOffSlider_valueChanged(int value)
{
    ui->CutOffVal->setNum(value);

    emit CutOffChanged(value);
}

void Dialog::on_OuterCutOffSlider_valueChanged(int value)
{
    ui->OuterCutOffVal->setNum(value);

    emit OuterCutOffChanged(value);
}

void Dialog::on_SpotLightColorButton_clicked()
{
    lightSource = LightSources::Spot;
    colorDialog->setCurrentColor(SpotLightColor);
    colorDialog->show();
}

void Dialog::on_AmbientColorButton_clicked()
{
    lightSource = LightSources::Ambient;
    colorDialog->setCurrentColor(AmbientColor);
    colorDialog->show();
}
