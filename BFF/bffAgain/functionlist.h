#ifndef FUNCTIONLIST_H
#define FUNCTIONLIST_H


class FunctionList
{
public:
    FunctionList();
    void func_0();//右下角功能:隐藏或显示桌面图标
    void func_1();//左下角功能
    void func_2();//左上角功能
    void func_3();//右上角功能
    ~FunctionList();

private:
    /*功能使能标志*/
    bool func0Flage;
    bool func1Flage;
};

#endif // FUNCTIONLIST_H
