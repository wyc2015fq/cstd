# 【C++常量折叠】改改const试试 _alibaba - CD's Coding - CSDN博客





2015年04月03日 01:42:54[糖果天王](https://me.csdn.net/okcd00)阅读数：671








今天阿里巴巴算法工程师实习生的笔试有这样一道题，问输出啥



```cpp
#include <iostream>
using namespace std;
int main(void)
{
    const int a = 10;
    int * p = (int *)(&a);
    *p = 20;
    cout<<"a = "<<a<<", *p = "<<*p<<endl;
    return 0;
}
```


当时我凭感觉觉得——肯定是10，20吧

考完之后尝试了一下编译运行果真如此，但是，很奇怪不是么，*p成20了我懂，但const的地址指向的地方都被改掉了呀，为啥a还是10呢？

于是……到处找找看资料学习下




constvolatileint i = 10;
  


constint i = 10;
  


原来如此，这两个是不一样的，C++中有个东西叫做常量折叠，




《定义》
c++里的常量折叠（或者常量替换)是将const常量放在[符号表](http://baike.baidu.com/view/5044878.htm)中，而并不给其分配内存。[编译器](http://baike.baidu.com/view/487018.htm)直接进行替换优化。




嘛，简单的说，就是如果直接 const int i = 10; 和 #define i 10差不多（嘛，微妙的有点区别，不过10也看作是个整型大差不差了）

实际上，在刚刚代码中*p=20的时候，确实const i所在地址的值被改成20了没错，但是会输出10，就是因为这个常量折叠的原因，当加上volatile（易变的）修饰符之后，系统就知道，嗯这个东西善变啊，输出的时候就不是从符号表中提取值而是乖乖从内存去读了，那样一来，就是20咯~~~




P.S》 不是我黑VC++6.0哦……它可能确实……有点老了。在这方面我们凭事实说话……

VS2010中，一切和上述说明一致，

DevC++中，一切和上述说明一致，

VC++6.0中，即便你加了volatile，也和没加一样输出10，20——

虽然定义const变量的同时加上了volatile修饰符，但VC++ 6.0编译器还是进行了优化措施，输出 i 时 从编译器的符号表中取值，直接输出。





constvolatileint i = 10;
  


constint i = 10;
  




