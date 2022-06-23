#include "SandboxMainWindow.h"
#include <iostream>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "DrawWidget.h"

SandboxMainWindow::SandboxMainWindow(QWidget *parent)
    : QMainWindow(parent/*,Qt::ToolTip*/)
{
    setupUi();
    openGLWidget->setLDataPtr(&m_LightData);
    connect(lSliderX, &QSlider::valueChanged, this, &SandboxMainWindow::xyzSliderChanged);
    connect(lSliderY, &QSlider::valueChanged, this, &SandboxMainWindow::xyzSliderChanged);
    connect(lSliderZ, &QSlider::valueChanged, this, &SandboxMainWindow::xyzSliderChanged);
    connect(lSliderKC, &QSlider::valueChanged, this, &SandboxMainWindow::attnSliderChanged);
    connect(lSliderKL, &QSlider::valueChanged, this, &SandboxMainWindow::attnSliderChanged);
    connect(lSliderKQ, &QSlider::valueChanged, this, &SandboxMainWindow::attnSliderChanged);
    connect(lSlidercutoff, &QSlider::valueChanged, this, &SandboxMainWindow::attnSliderChanged);
    connect(openGLWidget, &DrawWidget::sigClose, this, &SandboxMainWindow::close);
    connect(openGLWidget, &DrawWidget::sigToggleFullScreen, this, &SandboxMainWindow::toggleFullScreen);
    //trigger initial slider values
    xyzSliderChanged();
    attnSliderChanged();
}

void SandboxMainWindow::xyzSliderChanged()
{
    float sX = lSliderX->value()/4.0f;
    float sY = lSliderY->value()/4.0f;
    float sZ = lSliderZ->value()/4.0f;
    xLbl->setText(QString::number(sX));
    yLbl->setText(QString::number(sY));
    zLbl->setText(QString::number(sZ));
    m_LightData.pos = glm::vec3(sX, sY, sZ);
    openGLWidget->repaint();
}

void SandboxMainWindow::attnSliderChanged()
{
    float kC = lSliderKC->value() / 400.0f;
    float kL = lSliderKL->value() / 400.0f;
    float kQ = lSliderKQ->value() / 400.0f;
    float cutOff = lSlidercutoff->value() / 400.0f;
    KcLbl->setText(QString::number(kC));
    KlLbl->setText(QString::number(kL));
    KqLbl->setText(QString::number(kQ));
    cutOffLbl->setText(QString::number(cutOff));
    m_LightData.attenuation = glm::vec4(kC, kL, kQ, cutOff);
    openGLWidget->repaint();
}

void SandboxMainWindow::colorSliderChanged()
{
    float kC = lSliderKC->value() / 400.0f;
    float kL = lSliderKL->value() / 400.0f;
    float kQ = lSliderKQ->value() / 400.0f;
    KcLbl->setText(QString::number(kC));
    KlLbl->setText(QString::number(kL));
    KqLbl->setText(QString::number(kQ));
    m_LightData.attenuation = glm::vec4(kC, kL, kQ, 0);
    openGLWidget->repaint();
}

void SandboxMainWindow::toggleFullScreen()
{
    if (isFullScreen())
        showNormal();
    else
        showFullScreen();
}

void SandboxMainWindow::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("SandBoxMainWindow"));
    this->resize(600, 400);
    this->setMouseTracking(false);
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    centralWidget->setMouseTracking(false);
    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(1);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(1);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(1);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(1);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    xCap = new QLabel(centralWidget);
    xCap->setObjectName(QString::fromUtf8("xCap"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(xCap->sizePolicy().hasHeightForWidth());
    xCap->setSizePolicy(sizePolicy);
    xCap->setMinimumSize(QSize(15, 15));
    xCap->setMaximumSize(QSize(15, 15));
    xCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_3->addWidget(xCap);

    lSliderX = new QSlider(centralWidget);
    lSliderX->setObjectName(QString::fromUtf8("lSliderX"));
    lSliderX->setMinimumSize(QSize(0, 15));
    lSliderX->setMaximumSize(QSize(16777215, 15));
    lSliderX->setMinimum(-40);
    lSliderX->setMaximum(40);
    lSliderX->setOrientation(Qt::Horizontal);

    horizontalLayout_3->addWidget(lSliderX);

    xLbl = new QLabel(centralWidget);
    xLbl->setObjectName(QString::fromUtf8("xLbl"));
    sizePolicy.setHeightForWidth(xLbl->sizePolicy().hasHeightForWidth());
    xLbl->setSizePolicy(sizePolicy);
    xLbl->setMinimumSize(QSize(30, 15));
    xLbl->setMaximumSize(QSize(30, 15));
    xLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_3->addWidget(xLbl);


    verticalLayout_2->addLayout(horizontalLayout_3);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setSpacing(1);
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    yCap = new QLabel(centralWidget);
    yCap->setObjectName(QString::fromUtf8("yCap"));
    sizePolicy.setHeightForWidth(yCap->sizePolicy().hasHeightForWidth());
    yCap->setSizePolicy(sizePolicy);
    yCap->setMinimumSize(QSize(15, 15));
    yCap->setMaximumSize(QSize(15, 15));
    yCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_4->addWidget(yCap);

    lSliderY = new QSlider(centralWidget);
    lSliderY->setObjectName(QString::fromUtf8("lSliderY"));
    lSliderY->setMinimumSize(QSize(0, 15));
    lSliderY->setMaximumSize(QSize(16777215, 15));
    lSliderY->setMinimum(1);
    lSliderY->setMaximum(40);
    lSliderY->setValue(12);
    lSliderY->setOrientation(Qt::Horizontal);

    horizontalLayout_4->addWidget(lSliderY);

    yLbl = new QLabel(centralWidget);
    yLbl->setObjectName(QString::fromUtf8("yLbl"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(yLbl->sizePolicy().hasHeightForWidth());
    yLbl->setSizePolicy(sizePolicy1);
    yLbl->setMinimumSize(QSize(30, 15));
    yLbl->setMaximumSize(QSize(30, 15));
    yLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_4->addWidget(yLbl);


    verticalLayout_2->addLayout(horizontalLayout_4);

    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setSpacing(6);
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    zCap = new QLabel(centralWidget);
    zCap->setObjectName(QString::fromUtf8("zCap"));
    sizePolicy.setHeightForWidth(zCap->sizePolicy().hasHeightForWidth());
    zCap->setSizePolicy(sizePolicy);
    zCap->setMinimumSize(QSize(15, 15));
    zCap->setMaximumSize(QSize(15, 15));
    zCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_5->addWidget(zCap);

    lSliderZ = new QSlider(centralWidget);
    lSliderZ->setObjectName(QString::fromUtf8("lSliderZ"));
    lSliderZ->setMinimumSize(QSize(0, 15));
    lSliderZ->setMaximumSize(QSize(16777215, 15));
    lSliderZ->setMinimum(-40);
    lSliderZ->setMaximum(40);
    lSliderZ->setSingleStep(1);
    lSliderZ->setOrientation(Qt::Horizontal);

    horizontalLayout_5->addWidget(lSliderZ);

    zLbl = new QLabel(centralWidget);
    zLbl->setObjectName(QString::fromUtf8("zLbl"));
    sizePolicy.setHeightForWidth(zLbl->sizePolicy().hasHeightForWidth());
    zLbl->setSizePolicy(sizePolicy);
    zLbl->setMinimumSize(QSize(30, 15));
    zLbl->setMaximumSize(QSize(30, 15));
    zLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_5->addWidget(zLbl);


    verticalLayout_2->addLayout(horizontalLayout_5);


    horizontalLayout_2->addLayout(verticalLayout_2);

    verticalLayout_3 = new QVBoxLayout();
    verticalLayout_3->setSpacing(1);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    horizontalLayout_8 = new QHBoxLayout();
    horizontalLayout_8->setSpacing(1);
    horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
    kCCap = new QLabel(centralWidget);
    kCCap->setObjectName(QString::fromUtf8("kCCap"));
    sizePolicy.setHeightForWidth(kCCap->sizePolicy().hasHeightForWidth());
    kCCap->setSizePolicy(sizePolicy);
    kCCap->setMinimumSize(QSize(20, 15));
    kCCap->setMaximumSize(QSize(20, 15));
    kCCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_8->addWidget(kCCap);

    lSliderKL = new QSlider(centralWidget);
    lSliderKL->setObjectName(QString::fromUtf8("lSliderKL"));
    lSliderKL->setMinimumSize(QSize(0, 15));
    lSliderKL->setMaximumSize(QSize(16777215, 15));
    lSliderKL->setMinimum(1);
    lSliderKL->setMaximum(100);
    lSliderKL->setValue(50);
    lSliderKL->setOrientation(Qt::Horizontal);

    horizontalLayout_8->addWidget(lSliderKL);

    KlLbl = new QLabel(centralWidget);
    KlLbl->setObjectName(QString::fromUtf8("KlLbl"));
    sizePolicy1.setHeightForWidth(KlLbl->sizePolicy().hasHeightForWidth());
    KlLbl->setSizePolicy(sizePolicy1);
    KlLbl->setMinimumSize(QSize(30, 15));
    KlLbl->setMaximumSize(QSize(35, 15));
    KlLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_8->addWidget(KlLbl);


    verticalLayout_3->addLayout(horizontalLayout_8);

    horizontalLayout_7 = new QHBoxLayout();
    horizontalLayout_7->setSpacing(1);
    horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
    kLCap = new QLabel(centralWidget);
    kLCap->setObjectName(QString::fromUtf8("kLCap"));
    sizePolicy.setHeightForWidth(kLCap->sizePolicy().hasHeightForWidth());
    kLCap->setSizePolicy(sizePolicy);
    kLCap->setMinimumSize(QSize(20, 15));
    kLCap->setMaximumSize(QSize(20, 15));
    kLCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_7->addWidget(kLCap);

    lSliderKC = new QSlider(centralWidget);
    lSliderKC->setObjectName(QString::fromUtf8("lSliderKC"));
    lSliderKC->setMinimumSize(QSize(0, 15));
    lSliderKC->setMaximumSize(QSize(16777215, 15));
    lSliderKC->setMinimum(1);
    lSliderKC->setMaximum(100);
    lSliderKC->setValue(50);
    lSliderKC->setOrientation(Qt::Horizontal);

    horizontalLayout_7->addWidget(lSliderKC);

    KcLbl = new QLabel(centralWidget);
    KcLbl->setObjectName(QString::fromUtf8("KcLbl"));
    sizePolicy.setHeightForWidth(KcLbl->sizePolicy().hasHeightForWidth());
    KcLbl->setSizePolicy(sizePolicy);
    KcLbl->setMinimumSize(QSize(30, 15));
    KcLbl->setMaximumSize(QSize(35, 15));
    KcLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_7->addWidget(KcLbl);


    verticalLayout_3->addLayout(horizontalLayout_7);

    horizontalLayout_6 = new QHBoxLayout();
    horizontalLayout_6->setSpacing(1);
    horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
    kQCap = new QLabel(centralWidget);
    kQCap->setObjectName(QString::fromUtf8("kQCap"));
    sizePolicy.setHeightForWidth(kQCap->sizePolicy().hasHeightForWidth());
    kQCap->setSizePolicy(sizePolicy);
    kQCap->setMinimumSize(QSize(20, 15));
    kQCap->setMaximumSize(QSize(20, 15));
    kQCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_6->addWidget(kQCap);

    lSliderKQ = new QSlider(centralWidget);
    lSliderKQ->setObjectName(QString::fromUtf8("lSliderKQ"));
    lSliderKQ->setMinimumSize(QSize(0, 15));
    lSliderKQ->setMaximumSize(QSize(16777215, 15));
    lSliderKQ->setMinimum(1);
    lSliderKQ->setMaximum(50);
    lSliderKQ->setSingleStep(1);
    lSliderKQ->setValue(25);
    lSliderKQ->setOrientation(Qt::Horizontal);

    horizontalLayout_6->addWidget(lSliderKQ);

    KqLbl = new QLabel(centralWidget);
    KqLbl->setObjectName(QString::fromUtf8("KqLbl"));
    sizePolicy.setHeightForWidth(KqLbl->sizePolicy().hasHeightForWidth());
    KqLbl->setSizePolicy(sizePolicy);
    KqLbl->setMinimumSize(QSize(30, 15));
    KqLbl->setMaximumSize(QSize(35, 15));
    KqLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_6->addWidget(KqLbl);


    verticalLayout_3->addLayout(horizontalLayout_6);

    horizontalLayout_9 = new QHBoxLayout();
    horizontalLayout_9->setSpacing(1);
    horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
    cfCap = new QLabel(centralWidget);
    cfCap->setObjectName(QString::fromUtf8("cfCap"));
    sizePolicy.setHeightForWidth(cfCap->sizePolicy().hasHeightForWidth());
    cfCap->setSizePolicy(sizePolicy);
    cfCap->setMinimumSize(QSize(20, 15));
    cfCap->setMaximumSize(QSize(20, 15));
    cfCap->setAlignment(Qt::AlignCenter);

    horizontalLayout_9->addWidget(cfCap);

    lSlidercutoff = new QSlider(centralWidget);
    lSlidercutoff->setObjectName(QString::fromUtf8("lSlidercutoff"));
    lSlidercutoff->setMinimumSize(QSize(0, 15));
    lSlidercutoff->setMaximumSize(QSize(16777215, 15));
    lSlidercutoff->setMinimum(0);
    lSlidercutoff->setMaximum(400);
    lSlidercutoff->setSingleStep(1);
    lSlidercutoff->setValue(0);
    lSlidercutoff->setOrientation(Qt::Horizontal);

    horizontalLayout_9->addWidget(lSlidercutoff);

    cutOffLbl = new QLabel(centralWidget);
    cutOffLbl->setObjectName(QString::fromUtf8("cutOffLbl"));
    sizePolicy.setHeightForWidth(cutOffLbl->sizePolicy().hasHeightForWidth());
    cutOffLbl->setSizePolicy(sizePolicy);
    cutOffLbl->setMinimumSize(QSize(30, 15));
    cutOffLbl->setMaximumSize(QSize(35, 15));
    cutOffLbl->setAlignment(Qt::AlignCenter);

    horizontalLayout_9->addWidget(cutOffLbl);


    verticalLayout_3->addLayout(horizontalLayout_9);


    horizontalLayout_2->addLayout(verticalLayout_3);


    verticalLayout->addLayout(horizontalLayout_2);

    QGLFormat sf;
    sf.setSwapInterval(0);
    sf.setSamples(4);
    sf.setDoubleBuffer(true);
    openGLWidget = new DrawWidget(sf, centralWidget);
    openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
    openGLWidget->setSizePolicy(sizePolicy2);
    openGLWidget->setMouseTracking(false);
    openGLWidget->setFocusPolicy(Qt::StrongFocus);

    verticalLayout->addWidget(openGLWidget);

    this->setCentralWidget(centralWidget);

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
}

void SandboxMainWindow::retranslateUi()
{
    this->setWindowTitle(QApplication::translate("SandBoxMainWindow", "SandBox", nullptr));
    xCap->setText(QApplication::translate("SandBoxMainWindow", "X", nullptr));
    xLbl->setText(QApplication::translate("SandBoxMainWindow", "0", nullptr));
    yCap->setText(QApplication::translate("SandBoxMainWindow", "Y", nullptr));
    yLbl->setText(QApplication::translate("SandBoxMainWindow", "3", nullptr));
    zCap->setText(QApplication::translate("SandBoxMainWindow", "Z", nullptr));
    zLbl->setText(QApplication::translate("SandBoxMainWindow", "0", nullptr));
    kCCap->setText(QApplication::translate("SandBoxMainWindow", "kC", nullptr));
    KlLbl->setText(QApplication::translate("SandBoxMainWindow", "0.125", nullptr));
    kLCap->setText(QApplication::translate("SandBoxMainWindow", "kL", nullptr));
    KcLbl->setText(QApplication::translate("SandBoxMainWindow", "0.125", nullptr));
    kQCap->setText(QApplication::translate("SandBoxMainWindow", "kQ", nullptr));
    KqLbl->setText(QApplication::translate("SandBoxMainWindow", "0.125", nullptr));
    cfCap->setText(QApplication::translate("SandBoxMainWindow", "cf", nullptr));
    cutOffLbl->setText(QApplication::translate("SandBoxMainWindow", "0", nullptr));
}