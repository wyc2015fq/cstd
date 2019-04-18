# c_str函数 - nosmatch的专栏 - CSDN博客
2012年05月09日 10:16:56[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：436标签：[c																[string																[iostream																[语言																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
c_str函数的返回值是const char*的，不能直接赋值给char*，所以就需要我们进行相应的操作转化，下面就是这一转化过程。
　　c++语言提供了两种字符串实现，其中较原始的一种只是字符串的[c语言](http://baike.baidu.com/view/1219.htm)实现。与C语言的其他部分一样，它在c++的所有实现中可用，我们将这种实现提供的字符串对象，归为c-串，每个c-串char*类型的。
　　标准头文件<cstring>包含操作c-串的函数库。这些库函数表达了我们希望使用的几乎每种字符串操作。 当调用库函数，客户程序提供的是[string类](http://baike.baidu.com/view/3369697.htm)型参数，而库函数内部实现用的是c-串，因此需要将string对象，转化为char*对象，而c_str()提供了这样一种方法，它返回const char*类型(可读不可改)的指向字符[数组](http://baike.baidu.com/view/209670.htm)的指针。
 例：
　　#include <iostream>
　　#include <string>
　　using namespace std;
　　int main()
　　{
　　string add_to = "hello!";
　　const string add_on = "baby";
　　const char *cfirst = add_to.c_str();
　　const char *csecond = add_on.c_str();
　　char *copy = new char[strlen(cfirst) + strlen(csecond) + 1];
　　strcpy(copy, cfirst);
　　strcat(copy, csecond);
　　add_to = copy;
　　cout << "copy: " << copy << endl;
　　delete [] copy;
　　cout << "add_to: " << add_to << endl;
　　//我强烈建议，以及我的老师也强烈建议我
　　//一定要用return 0; 来告诉系统：程序正常结束.
　　//return -1;是用来告诉系统：程序异常结束
　　//亲们，做开发要认真~共勉~
　　//一定要用 int main()
　　return 0;
　　}  
