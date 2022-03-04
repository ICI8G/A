#include "functionlist.h"
#include <QDebug>
#include "hook.h"
#include <windows.h>


FunctionList::FunctionList()
{
    /*初始化功能使能标志*/
    func0Flage=false;
    func1Flage=false;
}


/***
 * 右下角功能:隐藏或显示桌面图标和任务栏
***/
void FunctionList::func_0()
{
    qDebug()<<"func0";


    HWND window = Hook::getFloderViewHWND();
    HWND tray = ::FindWindowExA(NULL, NULL,"Shell_TrayWnd",(char *)"");

    if(func0Flage)
    {
        ShowWindow(window, SW_SHOW);
        ShowWindow(tray, SW_SHOW);

    }
    else
    {
        ShowWindow(window, SW_HIDE);
        ShowWindow(tray, SW_HIDE);
    }

    func0Flage=!func0Flage;

    return;

}


/***
 * 左下角功能
***/
void FunctionList::func_1()
{
    qDebug()<<"func1";
}


/***
 * 左上角功能
***/
void FunctionList::func_2()
{
    qDebug()<<"func2";
}


/***
 * 右上角功能
***/
void FunctionList::func_3()
{
    qDebug()<<"func3";
}


FunctionList::~FunctionList()
{

}
