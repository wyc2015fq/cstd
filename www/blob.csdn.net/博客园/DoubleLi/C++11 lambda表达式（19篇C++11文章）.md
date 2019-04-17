# C++11 lambda表达式（19篇C++11文章） - DoubleLi - 博客园






C++11引入了lambda表达式，使得程序员可以定义匿名函数，该函数是一次性执行的，既方便了编程，又能防止别人的访问。

Lambda表达式的语法通过下图来介绍：



![](https://pic002.cnblogs.com/images/2012/306217/2012040822475657.jpg)

         这里假设我们定义了一个如上图的lambda表达式。现在来介绍途中标有编号的各个部分是什么意思。
- Lambda表达式的引入标志，在‘[]’里面可以填入‘=’或‘&’表示该lambda表达式“捕获”（lambda表达式在一定的scope可以访问的数据）的数据时以什么方式捕获的，‘&’表示一引用的方式；‘=’表明以值传递的方式捕获，除非专门指出。
- Lambda表达式的参数列表
- Mutable 标识
- 异常标识
- 返回值
- “函数”体，也就是lambda表达式需要进行的实际操作



将上图的代码片段补充完整：

    int x = 10;

    int y = 3;

    int z ;

    z = [=]()mutable throw() -> int { int n = x + y; x = y ; y = n; return n;}();

    cout<<z<<endl;

    cout<<"x:"<<x<<"\t"<<"y:"<<y<<endl;

运行结果为：

13

x: 10  y: 3

因为是以值传递的方式访问x,y所以x,y的值并没有发生改变



现在我们队lambda表达式的基本语法已经有一些了解，下面来举几个例子。





首先这个例子说明如何向lambda表达式里面传递参数：

#include <iostream>

using namespace std;

int main()

{

   int n = [] (int x, int y) { return x + y; }(5, 4);

   cout << n << endl;

}

运行结果为：9

通过这个例子我们可以看出，通过“函数体”后面的‘()’传入参数。



接下来这个例子可以看出，可以像调用函数一样使用lambda表达式，但是感觉这种方式和普通函数的定义与调用就差不多了，这里只是学习使用方式而已。

#include <iostream>

using namespace std;

int main()

{



   auto f = [] (int x, int y) { return x + y; };



   cout << f(21, 12) << endl;

}

运行结果为：33



Lambda表达式与STL算法一起使用，自己写测试代码的时候经常用到排序、输出数组什么的，通过下面列举的几个算法也比较方便：

#include <iostream>

#include <algorithm>

#include <ctime>

using namespace std;



int main()

{

    int a[10] = {0};



    srand(time(NULL));

    generate(a,a+10,[]()->int { return rand() % 100; });



    cout<<"before sort: "<<endl;

        for_each(a, a+10, [&](int i){ cout<< i <<" "; });



    cout<<endl;

    cout<<"After sort"<<endl;

    sort(a,a+10);

    for_each(a, a+10, [&](int i){ cout<< i <<" "; });

    return 0;

}



Lambda表达式的嵌套：

#include <iostream>

int main()

{

   using namespace std;



   int m = [](int x)

      { return [](int y) { return y * 2; }(x) + 3; }(5);



   cout << m << endl;

}

运行结果：13



以上代码在VC10和VC11上都能顺利编译通过。感觉lambda表达式还是比较有意思的语法，也是我接触的第一个VC11扩展。



http://www.cnblogs.com/zhuyp1015/archive/2012/04/08/2438176.html

http://www.cnblogs.com/zhuyp1015/category/370450.html









