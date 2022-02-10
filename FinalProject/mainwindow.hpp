#pragma once

#include "dialog.hpp"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SceneOptionsButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog* dialog;
};

