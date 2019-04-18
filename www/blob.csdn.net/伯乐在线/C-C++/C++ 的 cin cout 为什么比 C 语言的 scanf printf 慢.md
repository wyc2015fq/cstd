# C++ 的 cin/cout 为什么比 C 语言的 scanf/printf 慢 - 文章 - 伯乐在线
原文出处： [ZQ_onlytime](http://blog.csdn.net/ZQ_onlytime/article/details/44900081)
很早就知道，c的scanf（printf）比c++的快。刷题时尤其明显，在这上面超时是常有的事儿。
但，这是别人告诉我的，c快。
为什么快？
从网上借鉴一个例子做个简单测试：
1.cpp     //向一个文件里输入1000000个随机数

```
#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;
const int num=1000000;
int main()
{
    ofstream file("data");
    for(int i=0;i<num;i++)
    {
            file<<rand()<<' ';
            if((i+1)%20==0)
            file<<endl;
    }
    return 0;
}<a title="复制代码" target="_blank"><img src="http://jbcdn2.b0.upaiyun.com/2016/11/dfe484adc90388a238030ff29c7ec15b.gif" alt="复制代码" /></a>
```
2.cpp         //用cin读取这1000000个随机数

```
#include<iostream>
#include<ctime>
#include<cstdio>
#include<windows.h>
using namespace std;
const int num=1000000;
int main()
{
    freopen("data","r",stdin);
    int i,n,start,end;
    start=clock();
    for(i=0;i<num-2;i++)
    cin>>n;
    end=clock();
    cout<<double(end-start)/CLOCKS_PER_SEC<<endl;
    Sleep(5000);
    system("pause");
    return 0;
}
```
结果： 耗时  5.281秒
3.cpp       //用scanf读取这1000000个数

```
#include<ctime>
#include<cstdio>
#include<stdlib.h>
#include<windows.h>
#include<iostream>
using namespace std;
const int num=1000000;
int main()
{
    freopen("data","r",stdin);
    int i,n,start,end;
    start=clock();
    for(i=0;i<num;i++)
    scanf("%d",&n);
    end=clock();
    //cout<<double(end-start)/CLOCKS_PER_SEC<<endl;
    printf("%f\n",double(end-start)/CLOCKS_PER_SEC);
    system("pause");
    Sleep(5000);
    return 0;
}
```
结果： 耗时 0.437秒
结论：scanf真的比cin快。竟快10倍。
运行环境，xp，DEV-C++。
比较合理的解释：默认情况，cin与stdin总是保持同步的，也就是说这两种方法可以混用，而不必担心文件指针混乱，同时cout和stdout也一样，两者混用不会输 出顺序错乱。正因为这个兼容性的特性，导致cin有许多额外的开销，如何禁用这个特性呢？只需一个语句 std::ios::sync_with_stdio(false);，这样就可以取消cin于stdin的同步了，此时的cin就与scanf差不多 了。
另一种解释： cout在输出时总是要先将输出的存入缓存区。而printf直接调用系统进行IO，它是非缓存的。所以cout比printf慢。（这种解释，我没有验证）
scanf是格式化输入，printf是格式化输出。
cin是输入流，cout是输出流。效率稍低，但书写简便。
格式化输出效率比较高，但是写代码麻烦。
流输出操作效率稍低，但书写简便。
cout之所以效率低，正如一楼所说，是先把要输出的东西存入缓冲区，再输出，导致效率降低。
