#include "JaimeTutorial.h"

JaimeTutorial::JaimeTutorial(QWidget *parent)
    : QMainWindow(parent/*,Qt::ToolTip*/)
{
    ui.setupUi(this);
    ui.openGLWidget->setLDataPtr(&m_LightData);
    connect(ui.lSliderX, &QSlider::valueChanged, this, &JaimeTutorial::xyzSliderChanged);
    connect(ui.lSliderY, &QSlider::valueChanged, this, &JaimeTutorial::xyzSliderChanged);
    connect(ui.lSliderZ, &QSlider::valueChanged, this, &JaimeTutorial::xyzSliderChanged);
    connect(ui.lSliderKC, &QSlider::valueChanged, this, &JaimeTutorial::attnSliderChanged);
    connect(ui.lSliderKL, &QSlider::valueChanged, this, &JaimeTutorial::attnSliderChanged);
    connect(ui.lSliderKQ, &QSlider::valueChanged, this, &JaimeTutorial::attnSliderChanged);
    connect(ui.lSlidercutoff, &QSlider::valueChanged, this, &JaimeTutorial::attnSliderChanged);
    //trigger initial slider values
    xyzSliderChanged();
    attnSliderChanged();
}

void JaimeTutorial::xyzSliderChanged()
{
    float sX = ui.lSliderX->value()/4.0f;
    float sY = ui.lSliderY->value()/4.0f;
    float sZ = ui.lSliderZ->value()/4.0f;
    ui.xLbl->setText(QString::number(sX));
    ui.yLbl->setText(QString::number(sY));
    ui.zLbl->setText(QString::number(sZ));
    m_LightData.pos = glm::vec3(sX, sY, sZ);
    ui.openGLWidget->repaint();
}

void JaimeTutorial::attnSliderChanged()
{
    float kC = ui.lSliderKC->value() / 400.0f;
    float kL = ui.lSliderKL->value() / 400.0f;
    float kQ = ui.lSliderKQ->value() / 400.0f;
    float cutOff = ui.lSlidercutoff->value() / 400.0f;
    ui.KcLbl->setText(QString::number(kC));
    ui.KlLbl->setText(QString::number(kL));
    ui.KqLbl->setText(QString::number(kQ));
    ui.cutOffLbl->setText(QString::number(cutOff));
    m_LightData.attenuation = glm::vec4(kC, kL, kQ, cutOff);
    ui.openGLWidget->repaint();
}

void JaimeTutorial::colorSliderChanged()
{
    float kC = ui.lSliderKC->value() / 400.0f;
    float kL = ui.lSliderKL->value() / 400.0f;
    float kQ = ui.lSliderKQ->value() / 400.0f;
    ui.KcLbl->setText(QString::number(kC));
    ui.KlLbl->setText(QString::number(kL));
    ui.KqLbl->setText(QString::number(kQ));
    m_LightData.attenuation = glm::vec4(kC, kL, kQ, 0);
    ui.openGLWidget->repaint();
}
