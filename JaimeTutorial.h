#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_JaimeTutorial.h"
#include "lightData.h"

class JaimeTutorial : public QMainWindow
{
    Q_OBJECT

public:
    JaimeTutorial(QWidget *parent = Q_NULLPTR);

private:
    Ui::JaimeTutorialClass ui;
    lightData m_LightData;

private slots:
    void xyzSliderChanged();
    void attnSliderChanged();
    void colorSliderChanged();
};
