#pragma once

#include <QDialog>
#include <QColorDialog>
#include <QVector3D>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void AmbientColorChanged(QColor);
    void ObjectMaterialChanged(int);

    void DirLightIntensityChanged(float);
    void DirLightDirectionChanged(QVector3D);
    void DirLightColorChanged(QColor const&);
    void PointLightColorChanged(QColor const&);

    void PointLightIntensityChanged(float);
    void PointLightConstChanged(float);
    void PointLightLinChanged(float);
    void PointLightQuadChanged(float);

    void SpotLightColorChanged(QColor const&);
    void SpotLightIntensityChanged(float);
    void SpotLightConstChanged(float);
    void SpotLightLinChanged(float);
    void SpotLightQuadChanged(float);
    void CutOffChanged(float);
    void OuterCutOffChanged(float);

public slots:
    void addMaterial(QString const& name);
    void setLightColor(QColor const& color);

public slots:
    void on_MaterialComboBox_currentIndexChanged(int index);
    void on_DirLightXSpinBox_valueChanged(double arg1);
    void on_DirLightYSpinBox_valueChanged(double arg1);
    void on_DirLightZSpinBox_valueChanged(double arg1);
    void on_DirLightColorButton_clicked();
    void on_PointLightColorButton_clicked();
    void on_PointLightConstSpinBox_valueChanged(double arg1);
    void on_PointLightLinSpinBox_valueChanged(double arg1);
    void on_PointLightQuadSpinBox_valueChanged(double arg1);
    void on_SpotLightConstSpinBox_valueChanged(double arg1);
    void on_SpotLightLinSpinBox_valueChanged(double arg1);
    void on_SpotLightQuadSpinBox_valueChanged(double arg1);
    void on_CutOffSlider_valueChanged(int value);
    void on_OuterCutOffSlider_valueChanged(int value);

    void on_SpotLightColorButton_clicked();

    void on_AmbientColorButton_clicked();
public:

    enum class LightSources{
        Direct,
        Point,
        Spot,
        Ambient,
    };
    LightSources lightSource = LightSources::Direct;
    QVector3D dirLightDirection{0, -1, 0};
    Ui::Dialog *ui;
    QColorDialog* colorDialog;
    QColor AmbientColor;
    QColor DirLightColor;
    QColor PointLightColor;
    QColor SpotLightColor;
};

