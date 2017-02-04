#include <QApplication>
#include "taskwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TaskWindow taskWindow;
    a.exec();
    return 0;
}
