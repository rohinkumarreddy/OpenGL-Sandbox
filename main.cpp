#include "JaimeTutorial.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
#ifndef NDEBUG
    FreeConsole();
    AllocConsole();
    //AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
#endif
    QApplication a(argc, argv);
    JaimeTutorial w;
    w.show();
    return a.exec();
}
