# C++ 结构体实例和类实例的初始化 - 文章 - 伯乐在线
原文出处： [Eric_Jo](http://blog.csdn.net/eric_jo/article/details/4083806)
结构体实例（包括共用体）和类实例的初始化方法完全相同，二者都可以应用于继承层次中。不同点是结构体（包括共用体）默认成员为public，而类默认成员是private型的。
一、若类和结构体所有数据成员均为public型，可采取如下带花括号形式进行初始化。
注意：
①        不论值的个数多少，都必须使用花括号定界
②        未指定值的数据成员编译器会自动初始化为默认值
③        这种初始化对象方式，要求所有数据成员必须为public型
④        这种初始化对象方式，要求类中不能编写任何构造函数


```
struct S {  //class S 效果一样
    int            x;
    unsigned short y;
};
S testS1={100,123};
S testS2={200};//未指定值的数据成员初始化为默认值,这里os2.y=0;
S TestS[4]={ {100,10},
             {200,20},
             {300} };//未指定值的初始化为默认值,os[2].y,os[3].x,os[3].y。
```
二、若数据成员有private或protected型，或是提供了构造函数，必须使用构造函数来进行初始化。


```
struct S { //class S可自行试验，结果相同
    private:
        int x;
    public:
        double y;
        S(void){}
        S(int idemo,double ddemo) {x=idemo;y=ddemo;}
        void show(void) {cout<<x<<''/t''<<y<<endl;}
};
S os1;//将调用默认构造函数(无参构造函数)
S os2(1000,2.345);
S os3=S(2000,4.567);
S os[4]={S(10,1.234),S(20,2.234)};//未初始化的将调用默认构造函数。如此时没有默认构造函数会出错。
```
重要提示:
①        在S os3=S(2000,4.567);语句中，因是声明并初始化os3对象，所以将调用S(int,double)构造函数对os3进行初始化。
②        S os3(2000,4.567); 等价于 S os3=S(2000,4.567);
③        但如果os3已经存在了，S os3(100,1.234);os3=S(2000,4.567)，则表示用一个临时对象赋值给os3，将调用operator=，然后系统再释放这个临时产生的对象。系统默认的=运算是将源对象的数据成员的值复制到目标对象中的数据成员中。
三、接受一个参数的构造函数允许使用赋值句法初始化对象。
说明代码如下：

```
#include <iostream>
using namespace std;
class C {
    private:
        int x;
    public:
        C(int idemo) {x=idemo;}
        void show(void) {cout<<x<<endl;}
};
struct S {
    private:
        int x;
    public:
        S(int idemo) {x=idemo;}
        void show(void) {cout<<x<<endl;}
};
int main(int argc, char *argv[]){
    C oc=1000;//不能企图加花括号
    oc.show();
    S os=2000;//不能企图加花括号
    os.show();
    return EXIT_SUCCESS;
}
```
