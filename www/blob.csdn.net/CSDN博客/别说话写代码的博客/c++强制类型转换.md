# c++强制类型转换 - 别说话写代码的博客 - CSDN博客





2017年07月21日 09:32:13[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：98












#include <iostream>

using std::cout;

using std::endl;




int test0(void)

{

     double d1 = 3.33;




     int ival = (int)d1;

     int ival2 = int(d1);




     cout << "ival = " << ival << endl;

     cout << "ival2 = " << ival2 << endl;




     return 0;

}




int main(void)

{

     test0();




     double d1 = 3.33;

     int ival = static_cast<int>(d1);

     cout << "ival = " << ival << endl;




     // const_cast/dynamic_cast/reinterpret_cast都可进行强制类型转换

     return 0;

}


运行结果为：

![](https://img-blog.csdn.net/20170721093141374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







