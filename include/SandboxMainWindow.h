#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Sandbox.h"
#include "lightData.h"

class SandboxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SandboxMainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::SandBoxUIClass ui;
    lightData m_LightData;

private slots:
    void xyzSliderChanged();
    void attnSliderChanged();
    void colorSliderChanged();
};
