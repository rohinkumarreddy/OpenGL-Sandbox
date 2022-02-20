#include "SandboxMainWindow.h"
#include <QtWidgets/QApplication>

//#define RELEASE_DEBUG

#ifdef RELEASE_DEBUG
#define VLD_FORCE_ENABLE
#endif
#include <vld.h>

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
    SandboxMainWindow w;
    w.show();
    return a.exec();
}
