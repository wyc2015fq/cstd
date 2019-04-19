# C++中static - LC900730的博客 - CSDN博客
2017年09月01日 14:51:33[lc900730](https://me.csdn.net/LC900730)阅读数：108标签：[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
```
//testStatic.h
#ifndef TESTSTATIC_H
#define TESTSTATIC_H
#include<QObject>
#include<QDebug>
class TestStatic:public QObject
{
public:
    TestStatic();
    void testhello(){
        qDebug()<<"hello static";
    }
//类声明中的静态成员不能初始化
    static int number;
};
//在类外对静态属性初始化
int TestStatic::number=100;
//类静态方法可以class外声明+定义
static int Testhe(){
    return TestStatic::number;
}
#endif // TESTSTATIC_H
```
在其他文件中，可以直接调用静态方法，无需要加类名+前缀
```
//main.cpp
int main(int argc,char *argv[]){
    qDebug()<<Testhe();
}
```
静态方法中不能调用非静态属性
