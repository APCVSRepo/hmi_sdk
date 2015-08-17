#include "AppTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CAppTest w;
    w.show();

    return a.exec();
}
