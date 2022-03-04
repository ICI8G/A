#include "bff.h"
#include "ui_bff.h"
#include "qpainter.h"
#include "menuselect.h"
#include <QMouseEvent>
#include <windows.h>
#include "hook.h"
#include <QSoundEffect>
#include "functionlist.h"
#include <QMenu>
#include <QSettings>
#include <QDir>

#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"


BFF::BFF(QWidget *parent): QWidget(parent), ui(new Ui::BFF)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::SplashScreen);//设置无边框以及在任务栏不显示
    setAttribute(Qt::WA_TranslucentBackground);//实现透明窗口的设置

    /*设置音效*/
    homeEffect = new QSoundEffect;
    homeEffect->setSource(QUrl::fromLocalFile(":/res/home.wav"));
    homeEffect->setLoopCount(1);  //循环次数
    homeEffect->setVolume(1.0f); //音量  0~1之间

    /*设置音效*/
    doneEffect = new QSoundEffect;
    doneEffect->setSource(QUrl::fromLocalFile(":/res/done.wav"));
    doneEffect->setLoopCount(1);  //循环次数
    doneEffect->setVolume(1.0f); //音量  0~1之间

    this->setCursor(Qt::CrossCursor);//设置鼠标光标

    /*加载选择菜单的图片资源*/
    for(int i= 0; i<4; i++)
    {
        menuselect[i] = new MenuSelect(QString(":/res/%0.png").arg(i+1),QString(":/res/%0.png").arg(i+5),i);
        try
        {
        menuselect[i]->setParent(this);
        }
        catch(...)
        {
            qDebug()<<"setParentError";
        }
    }


    blank=13;//间隔
    normalSize=130;//开始大小
    largerSize=180;//放大大小

    functionList = new FunctionList;//功能列表


    autoStart=cheakAutoStart(name,path);//确认是否设置了开机自启动
    InitsystemTray();//初始化托盘




    /*建立关于音效的信号与槽的连接*/
    connect(this,SIGNAL(home()),SLOT(playHomeWav()));
    connect(this,SIGNAL(done()),SLOT(playDoneWav()));
    connect(menuselect[0],SIGNAL(selected()),menuselect[0],SLOT(playSelectWav()));
    connect(menuselect[1],SIGNAL(selected()),menuselect[1],SLOT(playSelectWav()));
    connect(menuselect[2],SIGNAL(selected()),menuselect[2],SLOT(playSelectWav()));
    connect(menuselect[3],SIGNAL(selected()),menuselect[3],SLOT(playSelectWav()));
}


/***
 * 初始化托盘
***/
void BFF::InitsystemTray()
{
    /*设置托盘*/
    systemTray = new QSystemTrayIcon(QIcon(":/res/mainIcon.png"));
    systemTray ->setToolTip(QString("寄!"));



    /*设置action*/
    actionQuit = new QAction();
    actionAtuoStart = new QAction();

    actionQuit->setIcon(QIcon(":/res/quitIcon.png"));
    actionAtuoStart->setIcon(QIcon(":/res/autoStartIcon.png"));


    /*设置菜单文本*/
    actionQuit->setText(QString("退出"));
    if(autoStart)
    {
        actionAtuoStart->setText(QString("开机自启动 √"));
    }
    else
    {
        actionAtuoStart->setText(QString("开机自启动"));
    }


    /*设置menu并加载action*/
    trayMenu = new QMenu();
    trayMenu->addAction(actionQuit);
    trayMenu->addSeparator();
    trayMenu->addAction(actionAtuoStart);

    /*加载menu并显示托盘*/
    systemTray->setContextMenu(trayMenu);
    systemTray->show();

    /*连接信号与槽*/
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(actionAtuoStart, SIGNAL(triggered()), this, SLOT(setAutoStart()));

}

/***
 * 发射退出信号
***/
void BFF::quit()
{
    qDebug()<<"quit";

    emit exeQuit();
}


/***
 * 设设置开机自启动或取消开机自启动
***/
void BFF::setAutoStart()
{

//    qDebug()<<autoStart;

    autoStart=!autoStart;

    if(autoStart)
    {
        actionAtuoStart->setText(QString("开机自启动 √"));
        settings->setValue(name,path);//设置自启动
    }
    else
    {
        actionAtuoStart->setText(QString("开机自启动"));
        settings->remove(name);//移除自启动
    }
}

/***
 *  确认是否设置了开机自启动
 *  @param &name 程序名
 *  @param &path 程序路径
 *  @return 返回是否设置了开机自启动
***/
bool BFF::cheakAutoStart(QString &name,QString &path)
{
    bool flag;

    settings = new QSettings(AUTO_RUN, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    QString applicationName = QApplication::applicationName();//获取应用名称
    QString oldPath = settings->value(applicationName).toString();


    //如果注册表中的路径和当前程序路径不一样或不存在，
    //则表示没有设置自启动或自启动程序已经更换了路径
    //toNativeSeparators的意思是将"/"替换为"\"

    QString appPath = QApplication::applicationFilePath();//找到应用的目录
    QString newPath = QDir::toNativeSeparators(appPath);


    if (oldPath != newPath)
    {
        flag=false;
    }
    else
    {
        flag=true;
    }

    name=applicationName;
    path=newPath;

    return flag;
}



/***
 * 播放home音效
***/
void BFF::playHomeWav()
{

    homeEffect->play();
}




/***
 * 播放done音效
***/
void BFF::playDoneWav()
{

    doneEffect->play();
}


/***
 * 重载paintEevnt，实现透明窗口
**/
void BFF::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /* 0x20为透明层颜色，可自定义设置为0x0到0xff */
    painter.fillRect(this->rect(), QColor(0, 0, 0, 0x01));
    return;
}



/***
 *  显示窗口gui
 *  @param event 鼠标移动事件
***/
void BFF::showUi(QMouseEvent *event)
{

    emit home();


    MenuSelect::setStartParam(-1);



    /*移动图片*/
    QPoint p;

    p.setX(event->globalPosition().x()+blank);
    p.setY(event->globalPosition().y()+blank);
    menuselect[0]->move(p);
    menuselect[0]->setInitPos(p);

    p.setX(event->globalPosition().x()-blank-normalSize);
    p.setY(event->globalPosition().y()+blank);
    menuselect[1]->move(p);
    menuselect[1]->setInitPos(p);


    p.setX(event->globalPosition().x()-blank-normalSize);
    p.setY(event->globalPosition().y()-blank-normalSize);
    menuselect[2]->move(p);
    menuselect[2]->setInitPos(p);

    p.setX(event->globalPosition().x()+blank);
    p.setY(event->globalPosition().y()-blank-normalSize);
    menuselect[3]->move(p);
    menuselect[3]->setInitPos(p);


    /*显示图片*/
    for(int i=0; i<4; i++)
    {
        menuselect[i]->setVisible(true);
    }

    update();

}



/***
 *  不显示窗口gui
***/
void BFF::hideUi()
{
    /*显示图片*/
    for(int i=0; i<4; i++)
    {
        menuselect[i]->setGeometry(0,0,normalSize,normalSize);
        menuselect[i]->setVisible(false);
    }
}


void BFF::keyReleaseEvent(QKeyEvent *event)
{

    /*当~松开时*/
    if(event->key()==Qt::Key_QuoteLeft && event->isAutoRepeat()==false)
    {
        this->showMinimized();
        this->hideUi();

        startFunction(MenuSelect::getStartParam());//执行相应功能


    }
    return;
}



/***
 * 执行鼠标所在的角的功能，也可以是除四角处即无动作发生
 * @param startParam 角的方位，右下开始为1，顺时针编号
***/
void BFF::startFunction(int startParam)
{
    if(startParam<0)return;

    qDebug()<<"done";

    emit done();

    switch(startParam)
    {
        case 0:
    {
        functionList->func_0();
    }break;

        case 1:
    {
        functionList->func_1();
    }break;


        case 2:
    {
        functionList->func_2();
    }break;

        case 3:
    {
        functionList->func_3();
    }break;


    }


}


/***
 * 析构函数
***/
BFF::~BFF()
{
    qDebug()<<"BFF quit";
    systemTray->hide();//隐藏托盘

    delete ui;
}




