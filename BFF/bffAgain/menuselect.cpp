#include "menuselect.h"
#include <QDebug>
#include <QVariantAnimation>
#include "hook.h"
#include <QSoundEffect>
#include "bff.h"


int MenuSelect::startParam=-1;//初始化静态成员变量， -1代表不属于任何角


/***
 * 构造函数
 * @param normalImage 初始图片路径
 * @param selectImage 放大后图片路径
 * @param id 用于识别时哪个角的控件
***/
MenuSelect::MenuSelect(QString normalImage,QString selectImage,int id)
{
    this->normalImage=normalImage;
    this->selectImage=selectImage;

    pix.load(normalImage);

    this->id=id;
    blank=13;//间隔
    normalSize=130;//开始大小
    largerSize=180;//放大大小
    flag = false;


    this->setStyleSheet("QLabel:{border:0px;}");//设置无边框
    this->setScaledContents(true);//允许等比放大
    this->setPixmap(pix.scaled(normalSize,normalSize,Qt::KeepAspectRatio));//允许等比放大
    this->setVisible(false);//初始化不可视
    //this->setMouseTracking(true);//开启鼠标追踪

    /*设置音效*/
    effect = new QSoundEffect;
    effect->setSource(QUrl::fromLocalFile(":/res/select.wav"));
    effect->setVolume(0.5f); //音量  0~1之间


    animation = new QPropertyAnimation(this, "geometry");//缩放动画

    /*在leave事件中的动画结束后图片重置为原始图片*/
    connect(animation, &QPropertyAnimation::finished, [=]
    {

        if(flag == true)
        {
            pix.load(normalImage);

            this->setPixmap(pix.scaled(normalSize,normalSize,Qt::KeepAspectRatio));

            flag=false;
        }

    });



}


/***
 * 播放音效
***/
void MenuSelect::playSelectWav()
{
    effect->play();

}


void MenuSelect::enterEvent(QEnterEvent *event)
{
//    qDebug()<<"enter";

    emit selected();

    startParam=id;


    /*播放动画*/

    pix.load(selectImage);

    this->setPixmap(pix.scaled(normalSize,normalSize,Qt::KeepAspectRatio));

    animation->setDuration(200);

    animation->setStartValue(QRect(initPos.x(),initPos.y(),normalSize,normalSize));
    animation->setEndValue(QRect(endPos.x(),endPos.y(),largerSize,largerSize));

    animation->setEasingCurve(QEasingCurve::InOutQuad);

    animation->start();



    return;
}


/***
 * 设置动画开始和结束时的位置
 * @param p 窗口的坐标
***/
void MenuSelect::setInitPos(QPoint p)
{
    initPos=p;

    switch(id)
    {
        case 0:
        {
            endPos=p;
        }break;

        case 1:
        {
            endPos=QPoint(p.x()+(normalSize-largerSize),p.y());
        }break;

        case 2:
        {
            endPos=QPoint(p.x()+(normalSize-largerSize),p.y()+(normalSize-largerSize));
        }break;

        case 3:
        {
            endPos=QPoint(p.x(),p.y()+(normalSize-largerSize));
        }break;
    }


}


void MenuSelect::leaveEvent(QEvent *event)
{
//    qDebug()<<"leave111";

    flag =true;

    if(Hook::isInBFF())
    {
        startParam=-1;
    }

    /*播放动画*/
    animation->setDuration(200);
    animation->setStartValue(QRect(endPos.x(),endPos.y(),largerSize,largerSize));
    animation->setEndValue(QRect(initPos.x(),initPos.y(),normalSize,normalSize));
    animation->setEasingCurve(QEasingCurve::InOutQuad);


    animation->start();


    return;
}

/***
 * 获得角的参考
 * @return 角的参考
***/
int MenuSelect::getStartParam()
{
//    qDebug()<<startParam;
    return startParam;

}

/***
 * 设置角的值
 * @param i -1为不属于任何角,从0代表右下开始 顺时针 至 3代表右上角
***/
void MenuSelect::setStartParam(int i)
{
    startParam=i;
}

