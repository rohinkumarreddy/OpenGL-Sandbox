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
  
    connect(openGLWidget, &DrawWidget::sigClose, this, &SandboxMainWindow::close);
    connect(openGLWidget, &DrawWidget::sigToggleFullScreen, this, &SandboxMainWindow::toggleFullScreen);
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
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    openGLWidget = new DrawWidget(centralWidget);
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
}