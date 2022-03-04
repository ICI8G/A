#ifndef HOOK_H
#define HOOK_H

#include <QDebug>
#include <windows.h>
#include <QThread>
#include "bff.h"

class Hook:public QThread
{

public:
    Hook(BFF *w);
    Hook();
    static HWND getFloderViewHWND();//得到存放桌面图标的句柄
    static HWND getMouseActivedHWND();//得到当前鼠标活动的窗口句柄
    LRESULT CALLBACK keyBoardHookProc(int nCode, WPARAM wParam, LPARAM lParam);//钩子，用于监听是否用鼠标中键在存放桌面图标的窗口中活动
    static bool isInBFF();//判断鼠标是否在主窗口内
    static HWND getDefViewHWND();//得到存放桌面图标的父句柄
    ~Hook();//析构函数 主要用于卸载钩子 其他的在关闭程序时会返回内存

private:
   void run();//重载虚函数，开启线程


};

#endif // HOOK_H
