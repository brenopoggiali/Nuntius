#include "mainwindow.h"
#include <QApplication>
#include "clientdialog.h"
#include "nickdialog.h"
#include <QTextBlock>

Q_DECLARE_METATYPE(QTextCursor)
Q_DECLARE_METATYPE(QTextCharFormat)
Q_DECLARE_METATYPE(QTextBlock)

int main(int argc, char *argv[])
{
    qRegisterMetaType<QTextCursor>("QTextCursor");
    qRegisterMetaType<QTextCharFormat>("QTextCharFormat");
    qRegisterMetaType<QTextBlock>("QTextBlock");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
