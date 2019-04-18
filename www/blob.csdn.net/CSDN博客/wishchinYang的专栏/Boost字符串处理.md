# Boost字符串处理 - wishchinYang的专栏 - CSDN博客
2013年09月27日 12:25:53[wishchin](https://me.csdn.net/wishchin)阅读数：790
[（1）：Boost学习之格式化输出--format](http://www.cnblogs.com/lzjsky/archive/2011/05/05/2037327.html)：
原文链接：[http://www.cnblogs.com/lzjsky/archive/2011/05/05/2037327.html](http://www.cnblogs.com/lzjsky/archive/2011/05/05/2037327.html)
此文非常详细！
**boost::format**类提供了类似C语言里'printf'功能的格式化输出能力，当然功能更强大。
### 所需头文件:
#include <boost/format.hpp>
### 示例代码:
```cpp
#include <iostream>
    #include <string>
    #include <boost/format.hpp>
     
    using namespace std;
    int _tmain(int argc, _TCHAR* argv[])
    {
        // 使用%序号%的方式给出指示符， 后面用%连接对应的数据。
        cout << boost::format("writing %1%,  x=%2% : %3%-th try") % "toto" % 40.23 % 50 << endl;
        // 输出:writing toto,  x=40.23 : 50-th try
       
        // 可以延迟使用，顺序不必一致
        boost::format fmter("%2% %1%");
        fmter % 36;
        fmter % 77;
        cout << fmter << endl;
        // 输出:77 36
       
        // 可重用
        fmter % 12;
        fmter % 24;
        cout << fmter << endl;
        // 输出:24 12
     
        // 可直接转成字符串
        std::string s = fmter.str();
        std::string s2 = str( boost::format("%2% %1% %2% %1%")%"World"%"Hello");
     
        cout << s << endl << s2 << endl;
        // 输出:
        // 24 12
        // Hello World Hello World
     
        // 可以使用printf指示符
        cout << boost::format("%3.1f - %.2f%%") % 10.0 % 12.5  << endl;
        // 输出:10.0 - 12.50%
     
        // printf指示符里使用N$指定使用第几个参数
        cout << boost::format("%2$3.1f - %1$.2f%%") % 10.0 % 12.5  << endl;
        // 输出:12.5 - 10.00%
     
        cin.get();
        return 0;
    }
```
### **boost::format**里的指示符语法大致有三大类:
#### 继承并强化自printf的格式化字符串
    形式为:[ N$ ] [ flags ] [ width ] [ . precision ] type-char
    N$可选，指定使用第N个参数（注意，要么所有指示符都加此参数，要么都不加）
    接下来的参数可以参数printf的指示符，只是format为其中的flags添加了'_'和'='标志，用于指出内部对齐和居中对齐。
#### 设置打印规则，它是printf参数的一个补充，只是为了更直观点。
    形式为:%|spec|
    如:%|1$+5|表示显示第一个参数，显示正负号，宽度为5
#### 简单的位置标记
    形式为:%N%
    简单地声明显示第N个参数，优点是比较直观而且不用指定类型。
（2）：C/C++的格式化输出(4)--boost::format
原文链接：http://blog.sina.com.cn/s/blog_a0d2e5590101ados.html
此文比较系统！
### **boost::format**里的指示符语法大致有四大类:
#### 继承并强化自printf的格式化字符串
   形式为:[N$
 ] [ flags ] [ width ] [ . precision ] type-char
   N$可选，指定使用第N个参数（注意，要么所有指示符都加此参数，要么都不加）
   接下来的参数可以参数printf的指示符，只是format为其中的flags添加了'_'和'='标志，用于指出内部对齐和居中对齐。
#### 设置打印规则，它是printf参数的一个补充，只是为了更直观点。
   形式为:%|spec|
   如:%|1$+5|表示显示第一个参数，显示正负号，宽度为5
#### 简单的位置标记
   形式为:%N%
   简单地声明显示第N个参数，优点是比较直观而且不用指定类型。
**boost::format新的格式说明符**
 %{nt} 
 当n是正数时，插入n个绝对制表符 
 cout <<boost::format("[t]")  <<endl; 
 %{nTX} 
 使用X做为填充字符代替当前流的填充字符（一般缺省是一个空格） 
 cout <<boost::format("[T*]")  <<endl; 
**下面是转载:**[http://www.cnblogs.com/WuErPIng/archive/2005/04/21/142308.html](http://www.cnblogs.com/WuErPIng/archive/2005/04/21/142308.html)
# [浅尝BOOST之FORMAT](http://www.cnblogs.com/WuErPIng/archive/2005/04/21/142308.html)
**概述      std::string是个很不错的东东，但实际使用时基本在每个程序里都会遇到不愉快的事情：格式化字符串。我甚至由于这个原因在代码里引入平台有关的MFC，ATL等本来不需要在项目中使用的一些重量级的框架，就为了能轻松的做格式化字符串:-)。曾尝试过将ATL::CString的format函数提取出来使用，但ATL::CString的底层调用了windows独有函数，无法跨越平台。当然，现在有了boost::format，我们不用再担心了。boost::format重载了'%'操作符，通过多次调用'%'操作符就能将参数非常方便格式化成字符串，并实现了ATL::CString和C#中的string两者的格式化字符串功能。除了语法刚开始感觉到怪异，功能足以让人感觉到兴奋！ **
**一、boost::format工作的方式**
 基本的语法，boost::format( format-string ) % arg1 %arg2 % ... % argN 
 下面的例子说明boost::format简单的工作方式  
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 方式一 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << boost::format("%s") % "输出内容" << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 方式二 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) std::string s; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) s = str(boost::format("%s") % "输出内容" ); 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << s << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 方式三 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) boost::formatformater("%s"); 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) formater % "输出内容"; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) std::string s = formater.str(); 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << s << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// 方式四 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << boost::format("%1%") % boost::io::group(hex,showbase, 40) << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
**二、boost::format实际使用的实例**
 格式化语法： [ N$ ] [ flags ] [ width ] [ . precision ]type-char  
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ATL::CString风格 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << boost::format("\n\n%s" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 十进制 =[%d]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化的十进制 =[]]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化十进制，前补'0' =[d]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 十六进制 =[%x]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 八进制 =[%o]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 浮点 =[%f]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化的浮点 =[%3.3f]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 科学计数 =[%e]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) ) % "example:\n" % 15 % 15 % 15 % 15 % 15 % 15.01 % 15.01 % 15.01 << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// C#::string风格 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << boost::format("%1%" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 十进制 =[%2$d]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化的十进制 =[%2$5d]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化十进制，前补'0' =[%2$05d]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 十六进制 =[%2$x]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 八进制 =[%2$o]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 浮点 =[%3$f]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 格式化的浮点 =[%3$3.3f]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) "%1t 科学计数 =[%3$e]\n" 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) ) % "example:\n" % 15 % 15.01 << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)输出结果 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
**三、boost::format新的格式说明符**
 %{nt} 
 当n是正数时，插入n个绝对制表符 
 cout <<boost::format("[t]")  <<endl; 
 %{nTX} 
 使用X做为填充字符代替当前流的填充字符（一般缺省是一个空格） 
 cout <<boost::format("[T*]")  <<endl;  
**四、异常处理**
 一般写法： 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){ 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) cout << boost::format("%d%d") % 1 << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif) }
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)catch(std::exception const & e) 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){ 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) cout << e.what() << endl; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// 输出内容： 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// boost::too_few_args:format-string
 refered to more arguments than werepassed 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif) }
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 boost::format的文档中有选择处理异常的办法，不过个人感觉实用性可能不强，下面是文档中的例子  
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// boost::io::all_error_bitsselects
 all errors 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// boost::io::too_many_args_bitselects
 errors due to passing too manyarguments. 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// boost::io::too_few_args_bitselects
 errors due to asking for the srting result before allarguments are passed 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) boost::formatmy_fmt(const std::string & f_string) 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){ 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)usingnamespace boost::io; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) formatfmter(f_string); 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) fmter.exceptions(all_error_bits ^ (too_many_args_bit | too_few_args_bit)
 ); 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return fmter; 
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif) }
![C/C++的格式化输出(4)--boost::format](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) cout << my_fmt(" %1%%2%\n") % 1 % 2 % 3 % 4 % 5;
**五、还有其它一些功能，但暂时感觉派不上用处，就不去深究了。**
（3）：char*,const char*和string的相互转换
原文链接：[http://blog.sina.com.cn/s/blog_5436b2f40100pjzz.html](http://blog.sina.com.cn/s/blog_5436b2f40100pjzz.html)
为了以后查找方便，特此总结如下。
如果有不对的地方或者有更简单的方法，请指出~~
```cpp
1.// string转const char*
   string s = "abc";
   const char* c_s =s.c_str();
 2.// const char*转string
   直接赋值即可
   const char* c_s ="abc";
   string s(c_s);
3. //string转char*
   string s = "abc";
   char* c;
   const int len =s.length();
   c = new char[len+1];
   strcpy(c,s.c_str());
4. //char*转string
   char* c = "abc";
   string s(c);
5. //const char*转char*
   const char* cpc ="abc";
   char* pc = newchar[100];//足够长
   strcpy(pc,cpc);
6. //char*转const char*
   //直接赋值即可
   char* pc = "abc";
   const char* cpc = pc;
```
(4):我的遍历代码
