#ifndef MENUSELECT_H
#define MENUSELECT_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QSoundEffect>
#include <QMediaPlayer>

class MenuSelect : public QLabel
{
    Q_OBJECT

public:
    MenuSelect(QString normalImage,QString selectImage,int id);
    void setInitPos(QPoint p);
    static int getStartParam();
    static void setStartParam(int i);

private:
    QPixmap pix;
    int id;
    int blank;//间隔
    int normalSize;//开始大小
    int largerSize;//放大大小
    QPoint initPos;//图标动画一开始存在的位置
    QPoint endPos;//图标动画结束时的位置
    bool flag;//判断是否时在enter事件中的标志
    QString normalImage,selectImage;//初始图片的路径，放大后图片的路径
    QSoundEffect* effect;//特效音效指针
    QThread* selectThread;//选择线程
    static int startParam;//角值 用于判断鼠标处于哪个位置  -1为不属于四角


protected:
    void enterEvent(QEnterEvent *event);//重载enter事件
    void leaveEvent(QEvent *event);//重载leave事件
    QPropertyAnimation *animation;//动画指针

signals:
    void selected();//代表鼠标悬浮于控件上的信号

public slots:
    void playSelectWav();//播放被选择时的音效的槽



};

#endif // MENUSELECT_H
