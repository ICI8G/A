#include "hook.h"
#include "bff.h"
#include <QDebug>
#include <windows.h>
#include <QApplication>
#include <QMouseEvent>


static HWND floderViewHWND=NULL;//存放桌面图标的句柄
static HWND windowHWND=NULL;//BFF柱窗口句柄
static HWND defView = NULL;
static HHOOK keyBoardHook=NULL;//钩子
static BFF *bff;



/***
 * 构造函数*w 主线程开启的主窗体指针
***/
Hook::Hook(BFF *w)
{
    bff=w;
}

Hook::Hook()
{

}

/***
 * 得到存放桌面图标的父句柄
 * @return 存放桌面图标的句柄
***/
HWND Hook::getDefViewHWND()
{
    HWND namehwnd,hwndWorkerW;//需要的子句柄
    HWND hwndDesktop = GetDesktopWindow();//桌面句柄
    hwndWorkerW=NULL;//初始为NULL，用于hWndChildAfter的后续迭代
    namehwnd=NULL;//初始为NULL

    //循环找到需要的子句柄
    while(namehwnd==NULL)
    {
        hwndWorkerW = ::FindWindowExA(hwndDesktop, hwndWorkerW, (char *)"WorkerW", (char *)"");
        namehwnd=::FindWindowExA(hwndWorkerW, NULL,(char *)"SHELLDLL_DefView",(char *)"");
    }

    return namehwnd;
}


/***
 * 得到存放桌面图标的句柄
 * @return 存放桌面图标的句柄
***/
HWND Hook::getFloderViewHWND()
{

    return ::FindWindowExA(defView, NULL, (char *)"SysListView32", (char *)"FolderView");//返回其句柄
}




/***
 * 得到当前鼠标活动的窗口句柄
 * @return 当前鼠标活动的窗口句柄
***/
HWND Hook::getMouseActivedHWND()
{
    POINT pNow = {0,0};
    GetCursorPos(&pNow);  // 获取鼠标当前位置

    return WindowFromPoint(pNow);
}



/***
 * 钩子，用于监听是否使用~键
 * @param nCode 执行方式
 * @param wParam 消息参数1
 * @param lParam 消息参数2
***/
LRESULT CALLBACK keyBoardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode == HC_ACTION)//需要执行时
    {

        KBDLLHOOKSTRUCT* p =(KBDLLHOOKSTRUCT*)lParam;

        if(wParam==WM_KEYDOWN && p->vkCode==VK_OEM_3 && ( floderViewHWND== Hook::getMouseActivedHWND() || defView == Hook::getMouseActivedHWND() ))//如果是在存放桌面图标的窗口中摁下
        {

            QMouseEvent event(QEvent::MouseButtonPress,QCursor::pos(),Qt::MiddleButton,Qt::MiddleButton,Qt::NoModifier);

            /*将主窗体至于前台并显示*/
            bff->showUi(&event);
            ShowWindow(windowHWND,SW_MAXIMIZE);
            SetForegroundWindow(windowHWND);
            SetFocus(windowHWND);

        }



    }
    return CallNextHookEx(NULL,nCode, wParam,lParam);
}


/***
 * 判断鼠标是否在主窗口内
 * @return 返回判断结果
***/
bool Hook::isInBFF()
{
    return getMouseActivedHWND()==windowHWND;
}




/***
 * 开启线程
***/
void Hook::run()
{
    try
    {
    defView = getDefViewHWND();
    floderViewHWND = getFloderViewHWND();//获得floderView的句柄
    windowHWND=::FindWindowExA(NULL,NULL,(char *)"Qt623QWindowIcon",(char *)"bff");//得到桌面的窗口的句柄


    if(!windowHWND)//如果找不到存放桌面的窗口的句柄
    {
        qDebug()<<"windowHWND";
    }

    keyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, ::keyBoardHookProc, NULL, 0);//安装钩子

    /*消息循环*/
    MSG msg;
    while(GetMessage(&msg,NULL,0,0))
    {

    }
    }
    catch(...)
    {
        qDebug()<<"runError";
    }
}


Hook::~Hook()
{
        UnhookWindowsHookEx(keyBoardHook);//卸载钩子
}


