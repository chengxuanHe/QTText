#include "widget.h"
#include "MyDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    MyDialog mydlg;
//    mydlg.show();
    return a.exec();
}
