# C++序列化使用 - wishchinYang的专栏 - CSDN博客
2014年07月02日 14:22:07[wishchin](https://me.csdn.net/wishchin)阅读数：3439
error C2248 无法访问私有成员 ：原因 ifstream 作为参数必须传引用！
**（1）：C++使用STL序列化**：原文链接：[http://blog.csdn.net/pandaxcl/article/details/649682](http://blog.csdn.net/pandaxcl/article/details/649682)
    在用C++编写应用程序的过程中，经常涉及到序列化的问题，但是序列化的问题通常都会有非常繁琐的过程代码需要书写，本文中就是通过简单的步骤实现了程序的序列化问题，简单直接，和其它的序列化方案有着很大的不同。    首先来看看简单的数据写入文件和从文件读入数据的代码：    特别注解：本人特别喜欢用STL来书写代码，一方面是便于移植，但是另一方却是在于用STL书写的代码简单直接，可读性好。如果还不熟悉STL，本文则不大适合你:)
```cpp
#endif
#if CODE1
////////////////////////////////////////////////////////////////////////////////
//模拟程序序列化的简单代码
#include <iostream>//cout
#include <fstream>//ofstream,ifstream
#include <vector>//vector
#include <iterator>//ostream_iterator,istream_iterator,back_inserter
#include <numeric>//partial_sum
#include <algorithm>//copy
#include <string>
#include <sstream>
using namespace std;//简化代码的书写，经std名字空间成为默认名字空间
int main()
{
    {//从程序序列化到文件
        vector<int> v(5,1);//[1,1,1,1,1]
        partial_sum(v.begin(),v.end(),v.begin());//[1,2,3,4,5]
        ofstream out("data.txt");//生成文件输出流
        //将数组v中的数据全部输出到文件流中，这种操作在C++中成为文件操作
        //在这里暂时称为序列化到文件操作。实际上这里为了简单序列化的格式
        //为文本文件格式。如果需要其它的格式完全可以通过自定义输出流游标
        //的方式，或者重载运算符operator<<和operator>>实现不同的序列化格
        //式。可以参见本人的其它相关文档。
        copy(v.begin(),v.end(),ostream_iterator<int>(out," "));
    }
    {//从文件序列化到程序
        vector<int> v;//模拟应用程序中数据
        ifstream in("data.txt");//建立输入流
        //下面的这行代码从文件中提取数据到v中，模拟了应用程序的序列化过程
        copy(istream_iterator<int>(in),istream_iterator<int>(),back_inserter(v));
        //下面的这行代码仅仅只是为了显示是否真的被序列化到了程序中
        copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
//该程序的输出如下：
/*******************************************************************************
1 2 3 4 5 
*******************************************************************************/
//data.txt中的内容如下：
/*******************************************************************************
1 2 3 4 5 
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
#endif//CODE1
```
**（2）：第二种方式：使用POST++ 类库**
信息科技第一门户：
原文链接：[http://www.atcto.net/Tech/C/2012-10-10/C,12101014524800000002.html](http://www.atcto.net/Tech/C/2012-10-10/C,12101014524800000002.html)
C++精彩博客系列：[http://blog.chinaunix.net/topic/Cjishu/](http://blog.chinaunix.net/topic/Cjishu/)
### （3） Boost.Serialization
链接介绍：[http://zh.highscore.de/cpp/boost/serialization.html](http://zh.highscore.de/cpp/boost/serialization.html)
轻量序列化介绍：[http://blog.csdn.net/fhxpp_27/article/details/8556948](http://blog.csdn.net/fhxpp_27/article/details/8556948)
Boost.Serialization可以创建或重建程序中的等效结构，并保存为二进制数据、文本数据、XML或者有用户自定义的其他文件。该库具有以下吸引人的特性：
- 代码可移植（实现仅依赖于ANSI C++）。 
- 深度指针保存与恢复。 
- 可以序列化STL容器和其他常用模版库。 
- 数据可移植。 
- 非入侵性。 
原文链接：[http://www.cnblogs.com/lanxuezaipiao/p/3703988.html](http://www.cnblogs.com/lanxuezaipiao/p/3703988.html)
（4）：使用**MessagePack**进行序列化
原文链接：[http://www.tuicool.com/articles/qMrayai](http://www.tuicool.com/articles/qMrayai)
msgpack官方主页：[http://msgpack.org/](http://msgpack.org/)
github主页：[https://github.com/msgpack/msgpack](https://github.com/msgpack/msgpack)
## 序列化自定义类型：msgpack已支持了很多的标准类型，但有时我们会自己定义新的类型，这时，我们必须对新类型做某些修改，以使msgpack可以操作它。
另外，如果你的类型中含有低层指针，则还需要进行一些处理，否则，msgpack只会进行浅拷贝，无法序列化指针所指向的内存数据。
假设我们原本的类型如下：
```cpp
struct Foo
{
    int  i;
    string  str;
    char*  data;
};
```
那么要让msgpack操作它，应修改为如下结构：
```cpp
struct Foo
{
    int     i;
    string  str;
    // 原始指针类型，内部封装了pack_raw和pack_raw_body方法
    msgpack::type::raw_ref  data;
    MSGPACK_DEFINE(i, str, data); 
};
 1 /*
 2  * msgpack C++试验：序列化/反序列化自定义数据结构.
 3  * Author: 赵子清
 4  * Blog: http://www.cnblogs.com/zzqcn
 5  * */
 6 
 7 
 8 #include <msgpack.hpp>
 9 #include <string>
10 #include <cstring>
11 #include <iostream>
12 using namespace std;
13 
14 
15 struct Foo
16 {
17     int     i;
18     string  str;
19     // 原始指针类型，内部封装了pack_raw和pack_raw_body方法
20     msgpack::type::raw_ref  data;
21 
22     MSGPACK_DEFINE(i, str, data); 
23 };
24 
25 
26 int main(int argc, char** argv)
27 {
28     Foo  f;
29     f.i = 4;
30     f.str = "hello world";
31     const char* tmp = "msgpack";
32     f.data.ptr = tmp;
33     f.data.size = strlen(tmp) + 1;
34 
35     msgpack::sbuffer  sbuf;
36     msgpack::pack(sbuf, f);
37 
38     msgpack::unpacked  unpack;
39     msgpack::unpack(&unpack, sbuf.data(), sbuf.size());
40 
41     msgpack::object  obj = unpack.get();
42 
43     Foo f2;
44     obj.convert(&f2);
45 
46     cout << f2.i << ", " << f2.str << ", ";
47     cout << f2.data.ptr << endl;
48 
49     return 0;
50 }
```
 输出结果：
4, hello world, msgpack
（5）：**自己编写**序列化代码
由于C++库的开放性差，弹性小，不能用于自己的结构体map，最后还是自己专门为自己的结构体写了序列化代码
