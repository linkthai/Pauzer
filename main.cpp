#include "minipauzer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MiniPauzer w;
    w.show();

    return a.exec();
}
