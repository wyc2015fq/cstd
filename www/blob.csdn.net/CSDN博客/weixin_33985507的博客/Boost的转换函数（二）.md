# Boost的转换函数（二） - weixin_33985507的博客 - CSDN博客
2011年08月06日 19:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
原文地址：[http://www.cnblogs.com/TianFang/archive/2008/09/29/1301880.html](http://www.cnblogs.com/TianFang/archive/2008/09/29/1301880.html)
- **boost::numeric_cast**
在c++中，我们经常需要把不同类型的数字互相转换，如将一个数字在long和short之间转换。但由于各数字的精度不同，当一个数字从"大"类型到"小"类型就可能导致转换失败，如下所示：
long n1 = 99999999;
short n2 = static_cast<short>(n1);
对于如上转换，n2得到的是一个负数，显然这个不是我们所期望的，并且这种运行时的错误是很难检测的，一旦使用了这个错误的转换后的数据，后果不堪设想。
boost::numeric_cast可以帮助我们解决这一问题，对于上面的转换，boost::numeric_cast会抛出一个boost:: bad_numeric_cast这个异常对象。从而保证转换后值的有效性。上述代码可以改写为如下：
try
{
long n1 = 99999999;
short n2 = boost::numeric_cast<short>(n1);
}
catch(boost::bad_numeric_cast&)
{ 
    std::cout<<"The conversion failed"<<std::endl; 
} 
numeric_cast是如何知道这样的数字转换失败的呢？numeric_cast合理的应用了std::numeric_limits<>，而std::numeric_limits<>就是内建数字类型的type_tratis。当然也可以将自己定义的数字抽象类型添加到std::numeric_limits<>的特化版本中，这样numeric_cast就可以作用于自定义的类型了。由于相对复杂点，本文是介绍其功能和用法，就不分析其源码了，感兴趣的朋友可以参看boost文档和代码。
对于numeric_cast的使用也是有些要求的。
- 
源类型和目标类型必须都是可拷贝构造的
- 
源类型和目标类型必须都是数字型类型。也就是被std::numeric_limits<>::is_specialized的特化定义为true
- 
源类型必须能被static_cast转换为目标类型
其实对我们用的系统内置的数字来说，这几条都不是限制，只有我们在需要通过它转换自定义的数据类型时，才需要注意，否则编译不通过（其实这个错误还比较好发现和解决）。
- **boost::lexical_cast**
在C/C++程序开发中，往往需要将数字型对象的值转换为字符文本格式，或反之操作。虽然C语言就提供了不少系统函数来进行这种操作，如scanf、atoi等。这些函数小巧简洁，使用很方便，但缺少扩展性。在std中引入了stringstream来以一个通用的方式实现各种转换，但缺少对错误转换的检测。而boost::lexical_cast是在stringstream上的一个扩展，增加了对错误的类型转换的检测：
#include<string>
#include<iostream>
#include<boost/lexical_cast.hpp>
usingnamespace std;
int main()
{     
try
    { 
int i = 100; 
        string str = boost::lexical_cast<string>(i); 
        cout<<"The string is:"<<str<<endl; 
        str = "error"; 
        i = boost::lexical_cast<int>(str); 
    } 
catch(boost::bad_lexical_cast& exobj)
    { 
        cout<<"Convert err:"<<endl; 
        cout<<exobj.what()<<endl; 
    } 
}
在上述转换中，第二个转换从"err"到int的转换是失败的，会抛出一个boost::bad_lexical_cast的异常，从而能帮助我们构造更安全稳定的程序。
boost::lexical_cast内部实现其实也是一个stringstream的封装，其函数简化如下：
template<typename Target,typename Source> 
Target lexical_cast(Source arg){ 
    detail::lexical_stream<Target,Source> interpreter; 
    Target result; 
if(!(interpreter<<arg && interpreter>>result)) 
        throw_exception(bad_lexical_cast(typeid(Target),typeid(Source))); 
return result; 
} 
其中lexical_stream<>对字符串流做了一系列的包装，主要提供了operator<<(Source)和operator>>(Target)操作，用于判断操作是否成功。
完
