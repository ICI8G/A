#ifndef BFF_H
#define BFF_H

#include <QWidget>
#include "menuselect.h"
#include <QSoundEffect>
#include "functionlist.h"
#include <QSystemTrayIcon>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class BFF; }
QT_END_NAMESPACE

class BFF : public QWidget
{
    Q_OBJECT

public:
    BFF(QWidget *parent = nullptr);

    MenuSelect *menuselect[4];//存放选择菜单的指针数组

    void showUi(QMouseEvent *event);//显示菜单
    void hideUi();//隐藏菜单

    ~BFF();

private:
    Ui::BFF *ui;
    int blank;//间隔
    int normalSize;//开始大小
    int largerSize;//放大大小
    QSoundEffect *homeEffect;//特效音效指针
    QSoundEffect *doneEffect;//特效音效指针
    FunctionList *functionList;//四角实现的功能函数
    QSystemTrayIcon* systemTray;//系统托盘
    QMenu* trayMenu;//托盘菜单
    QAction* actionQuit;//菜单退出动作
    QAction* actionAtuoStart;//菜单开机自启动动作
    QSettings *settings;//用于设定注册表
    QString name,path;//程序名和程序路径
    bool autoStart;//是否设置了开机自启动的标志

    void InitsystemTray();//初始化图片
    bool cheakAutoStart(QString &name,QString &path);//确定是否设置了开机自启动的标志
    void startFunction(int startParam);//执行四角的函数

protected:
    void paintEvent(QPaintEvent *);//重载paintEevnt，实现透明窗口
    void keyReleaseEvent(QKeyEvent *event);//重载鼠标事件

signals:
    void home();//代表切换到主窗口的信号
    void done();//代表开始执行功能的信号
    void exeQuit();//代表执行程序退出的信号

private slots:
    void playHomeWav();//代表播放切换到主窗口音效的槽
    void playDoneWav();//代表播放开始执行功能音效的槽
    void setAutoStart();//设置开机自启动或取消开机自启动的槽
    void quit();//代表执行程序退出的槽

};
#endif // BFF_H
