#include "zpomodoro.h"
#include "common.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationDisplayName(APP_NAME);
    a.setOrganizationName(ORG_NAME);
    a.setDesktopFileName(APP_NAME);
    a.setApplicationVersion( APP_VERSION );

    QFile f(":/style/style.css");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream t(&f);
    a.setStyleSheet(t.readAll());

    ZPomodoro w;
    w.show();

    return a.exec();
}
