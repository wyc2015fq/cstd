# c++默认参数 c++重载 - 别说话写代码的博客 - CSDN博客





2017年07月21日 09:33:32[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：134
个人分类：[c/c++](https://blog.csdn.net/qq_21997625/article/category/6988647)













#include <iostream>

using std::cout;

using std::endl;




//默认参数的设置只能采用从右到左的顺序

int add(int a=1,int b=0)

{

     return a+b;

}




//在设置默认参数时，要尽量避免二义性

int add(int a,int b,int c)

{

     return a+b+c;

}




int main(void)

{

     int a=3,b=4;




     cout << add(a,b) << endl;

     cout << add(a) << endl;

     cout << add()<<endl;




     return 0;

}


运行截图：






![](https://img-blog.csdn.net/20170721093355087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






#include <iostream>

using std::cout;

using std::endl;




//c++支持函数重载

//原理是名字改编：当函数名称相同时，会根据函数参数的类型，个数，顺序进行改编




int add(int a,int b)

{

     return a+b;

}

int add(int a,int b,int c)

{

     return a+b+c;

}




double add(double a,double b,double c)

{

     return a+b+c;

}




int add(double a,int b,char c)

{

     return 1;

}

int add(int a,double b,char c)

{

     return 2;

}




int main()

{

     int a=3,b=4,c=5;




     cout << "add(a,b) = " <<add(a,b) <<endl;




     cout << "add(a,b,c) = " << add(a,b,c) <<endl;

     cout << "add(double,double,double) = "<<add(1.1,2.2,3.33)<<endl;

     cout << "add(double,int,char) = "<< add(1.2,2,'c')<<endl;




     return 0;

}





运行结果：

![](https://img-blog.csdn.net/20170721093401724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






