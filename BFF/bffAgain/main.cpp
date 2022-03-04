#include "bff.h"
#include <windows.h>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "hook.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "bffAgain_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    /* 用于分离桌面*/
    HWND  hwnd=::FindWindowExA(NULL, NULL,(char *)"Progman",(char *)"Program Manager");
    SendMessageTimeout(hwnd,0x052C,0,NULL,SMTO_NORMAL,1000,NULL);

    BFF w;
    w.showFullScreen();//先全屏显示一下


    try
    {
        /*挂上钩子*/
        Hook *hook = new Hook(&w);
        hook->start();

    }
    catch (...)
    {
        qDebug()<<"error";
    }

    w.showMinimized();//在开启线程后最小化窗口

    QObject::connect(&w,SIGNAL(exeQuit()),&a,SLOT(quit()));

    return a.exec();
}
