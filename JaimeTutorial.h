#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_JaimeTutorial.h"

class JaimeTutorial : public QMainWindow
{
    Q_OBJECT

public:
    JaimeTutorial(QWidget *parent = Q_NULLPTR);

private:
    Ui::JaimeTutorialClass ui;
};
