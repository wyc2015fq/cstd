# memset structure with std::string contained - 三少GG - CSDN博客
2014年08月31日 19:39:56[三少GG](https://me.csdn.net/scut1135)阅读数：906
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

struct结构体含有 string类型，
memset(&xx, 0, sizeof(xx)); 访问xx.string导致段错误， 程序异常。花了好久才排查出此问题，谨记！ 
不可用memset来初始化带有string类型的struct ，可用**构造函数**来实现！！！
[http://stackoverflow.com/questions/6877281/memset-structure-with-stdstring-contained](http://stackoverflow.com/questions/6877281/memset-structure-with-stdstring-contained)
I have a massive structure where the entire contents is scalar variables, enumerations, and arrays of scalars (stack-based) with the exception of one std::string variable.
Now, here's my question...
Can I memset the structure to 0 for its whole size (like I would if it was simply all scalars), or is that not possible with the std::string being in there? I'm not sure what memset would do to its internal representation.
And if you're going to say its good/bad please explain why - I'd like to know why it is the way it is :)
**No, you cant, it would overwrite the internal state of the string and make bad things happen. You could wrap all the POD stuff in a seperate struct and put that in your current one, that way you could memset that and let the string default construct.**
**Edit: Just to clarify, the string will almost certainly be storing a pointer to the memory its allocated for storage. The string's constructor will always have run before you can memset it (even if you memset `this` in the constructor of your type, the string constructor would run first). So you would be overwriting this pointer value, and instead of pointing to its storage, it would a pointer to NULL, or some other almost definitely invalid value.**
*********************
# [struct的初始化及其构造函数](http://blog.csdn.net/niuox/article/details/7253374)
今天看数据结构时，突然看到一个用构造函数进行初始化的struct结构体，这个比较新鲜，因为以前从来没有看到过，于是想了想struct到底有没有构造函数以及struct如何初始化，上网查了一些资料，供大家参考！
结构体实例（包括共用体）和类实例的初始化方法完全相同，二者都可以应用于继承层次中。不同点是结构体（包括共用体）默认成员为public，而类默认成员是private型的。
一、若类和结构体所有数据成员均为public型，可采取如下带花括号形式进行初始化。
注意：
① 不论值的个数多少，都必须使用花括号定界
② 未指定值的数据成员编译器会自动初始化为默认值
③ 这种初始化对象方式，要求所有数据成员必须为public型
④ 这种初始化对象方式，要求类中不能编写任何构造函数
struct S {  //class S 效果一样
    int            x;
    unsigned short y;
};
S testS1={100,123};
S testS2={200};//未指定值的数据成员初始化为默认值,这里os2.y=0;
S TestS[4]={ {100,10},
             {200,20},
             {300} };//未指定值的初始化为默认值,os[2].y,os[3].x,os[3].y。
《windows核心编程》中我们会遇到：STARTUPINFO si = { sizeof(si) } ，就是对si结构体的初始化。
二、若数据成员有private或protected型，或是提供了构造函数，必须使用构造函数来进行初始化。
struct S { //class S可自行试验，结果相同
    private:
        int x;
    public:
        double y;
**        S(void){}        S(int idemo,double ddemo) {x=idemo;y=ddemo;}**
        void show(void) {cout<<x<<''\t''<<y<<endl;}
};
S os1;//将调用默认构造函数(无参构造函数)
S os2(1000,2.345);
S os3=S(2000,4.567);
S os[4]={S(10,1.234),S(20,2.234)};//未初始化的将调用默认构造函数。如此时没有默认构造函数会出错。
重要提示:
①在S os3=S(2000,4.567);语句中，因是声明并初始化os3对象，所以将调用S(int,double)构造函数对os3进行初始化。
②S os3(2000,4.567); 等价于 S os3=S(2000,4.567);
③但如果os3已经存在了，S os3(100,1.234);os3=S(2000,4.567)，则表示用一个临时对象赋值给os3，将调用operator=，然后系统再释放这个临时产生的对象。系统默认的=运算是将源对象的数据成员的值复制到目标对象中的数据成员中。
三、接受一个参数的构造函数允许使用赋值句法初始化对象。
说明代码如下：
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
原创文章如转载，请注明：转载自：[Outshine的个人网站](http://www.seuoutshine.com/) —[http://www.seuoutshine.com/](http://www.seuoutshine.com/)
本文链接地址：[http://www.seuoutshine.com/reed/structDeChuShiHuaJiQiGouZaoHanShu.html](http://www.seuoutshine.com/reed/structDeChuShiHuaJiQiGouZaoHanShu.html)
