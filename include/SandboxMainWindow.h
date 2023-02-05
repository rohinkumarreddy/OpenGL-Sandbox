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
    DrawWidget* openGLWidget;
    void setupUi();
    void retranslateUi();

private slots:
    void toggleFullScreen();
};
