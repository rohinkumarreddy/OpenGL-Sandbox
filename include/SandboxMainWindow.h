#pragma once

#include <QtWidgets/QMainWindow>
#include "lightData.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QSlider;
class DrawWidget;
QT_END_NAMESPACE

class SandboxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SandboxMainWindow(QWidget *parent = Q_NULLPTR);

private:
    lightData m_LightData;
    QWidget* centralWidget;
    QVBoxLayout* verticalLayout;
    QHBoxLayout* horizontalLayout;
    QHBoxLayout* horizontalLayout_2;
    QVBoxLayout* verticalLayout_2;
    QHBoxLayout* horizontalLayout_3;
    QLabel* xCap;
    QSlider* lSliderX;
    QLabel* xLbl;
    QHBoxLayout* horizontalLayout_4;
    QLabel* yCap;
    QSlider* lSliderY;
    QLabel* yLbl;
    QHBoxLayout* horizontalLayout_5;
    QLabel* zCap;
    QSlider* lSliderZ;
    QLabel* zLbl;
    QVBoxLayout* verticalLayout_3;
    QHBoxLayout* horizontalLayout_8;
    QLabel* kCCap;
    QSlider* lSliderKL;
    QLabel* KlLbl;
    QHBoxLayout* horizontalLayout_7;
    QLabel* kLCap;
    QSlider* lSliderKC;
    QLabel* KcLbl;
    QHBoxLayout* horizontalLayout_6;
    QLabel* kQCap;
    QSlider* lSliderKQ;
    QLabel* KqLbl;
    QHBoxLayout* horizontalLayout_9;
    QLabel* cfCap;
    QSlider* lSlidercutoff;
    QLabel* cutOffLbl;
    DrawWidget* openGLWidget;
    void setupUi();
    void retranslateUi();

private slots:
    void xyzSliderChanged();
    void attnSliderChanged();
    void colorSliderChanged();
    void toggleFullScreen();
};
