# sscanf函数的使用 - ljx0305的专栏 - CSDN博客
2010年01月04日 13:24:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：702标签：[正则表达式																[buffer																[c																[string																[扩展																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)
个人分类：[C																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/394717)
# sscanf
**名称:**
　　sscanf() - 从一个字符串中读进与指定格式相符的数据. 
**函数原型:**
　　Int sscanf( const char *, const char *, ...);
　　int scanf( const char *, ...); 
**头文件：**
**#include<stdio.h>**
**说明：**
　　sscanf与scanf类似，都是用于输入的，只是后者以键盘(stdin)为输入源，前者以固定字符串为输入源。
　　第一个参数可以是一个或多个 {%[*] [width] [{h | l | I64 | L}]type | ' ' | '/t' | '/n' | 非%符号} 
**注：**
　　1、 * 亦可用于格式中, (即 %*d 和 %*s) 加了星号 (*) 表示跳过此数据不读入. (也就是不把此数据读入参数中) 
　　2、{a|b|c}表示a,b,c中选一，[d],表示可以有d也可以没有d。 
　　3、width表示读取宽度。 
　　4、{h | l | I64 | L}:参数的size,通常h表示单字节size，I表示2字节 size,L表示4字节size(double例外),l64表示8字节size。
　　5、type :这就很多了，就是%s,%d之类。 
　　6、特别的：%*[width] [{h | l | I64 | L}]type 表示满足该条件的被过滤掉，不会向目标参数中写入值
**支持集合操作：**
　　%[a-z] 表示匹配a到z中任意字符，贪婪性(尽可能多的匹配)
　　%[aB'] 匹配a、B、'中一员，贪婪性
　　%[^a] 匹配非a的任意字符，贪婪性
[[编辑本段](http://writeblog.csdn.net/#)]
例子：
　　1. 常见用法。
　　char buf[512] = ;
　　sscanf("123456 ", "%s", buf);
　　printf("%s/n", buf);
　　结果为：123456
　　2. 取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。
　　sscanf("123456 ", "%4s", buf);
　　printf("%s/n", buf);
　　结果为：1234 
　　3. 取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。
　　sscanf("123456 abcdedf", "%[^ ]", buf);
　　printf("%s/n", buf);
　　结果为：123456 
　　4. 取仅包含指定字符集的字符串。如在下例中，取仅包含1到9和小写字母的字符串。
　　sscanf("123456abcdedfBCDEF", "%[1-9a-z]", buf);
　　printf("%s/n", buf);
　　结果为：123456abcdedf 
　　5. 取到指定字符集为止的字符串。如在下例中，取遇到大写字母为止的字符串。
　　sscanf("123456abcdedfBCDEF", "%[^A-Z]", buf);
　　printf("%s/n", buf);
　　结果为：123456abcdedf 
　　6、给定一个字符串iios/12DDWDFF@122，获取 / 和 @ 之间的字符串，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中
　　sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]", buf);
　　printf("%s/n", buf);
　　结果为：12DDWDFF 
　　7、给定一个字符串““hello, world”，仅保留world。（注意：“，”之后有一空格）
　　sscanf(“hello, world”, "%*s%s", buf); 
　　printf("%s/n", buf); 
　　结果为：world
　　%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了
　　如果没有空格则结果为NULL。
　　sscanf的功能很类似于正则表达式, 但却没有正则表达式强大,所以如果对于比较复杂的字符串处理,建议使用正则表达式.
　　//-------------------------------------------------------
　　sscanf,表示从字符串中格式化输入 
　　上面表示从str中，输入数字给x，就是32700 
　　久以前，我以为c没有自己的split string函数，后来我发现了sscanf；一直以来，我以为sscanf只能以空格来界定字符串，现在我发现我错了。 
　　sscanf是一个运行时函数，原形很简单： 
　　int sscanf( 
　　const char *buffer, 
　　const char *format [, 
　　argument ] ... 
　　); 
　　它强大的功能体现在对format的支持上。 
　　我以前用它来分隔类似这样的字符串2006:03:18: 
　　int a, b, c; 
　　sscanf("2006:03:18", "%d:%d:%d", a, b, c); 
　　以及2006:03:18 - 2006:04:18: 
　　char sztime1[16] = "", sztime2[16] = ""; 
　　sscanf("2006:03:18 - 2006:04:18", "%s - %s", sztime1, sztime2); 
　　但是后来，我需要处理2006:03:18-2006:04:18 
　　仅仅是取消了‘-’两边的空格，却打破了%s对字符串的界定。 
　　我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf替换成我自己的分割函数。我以为我肯定需要这样做，并伴随着对sscanf的强烈不满而入睡；一觉醒来，发现其实不必。 
　　format-type中有%[]这样的type field。如果读取的字符串，不是以空格来分隔的话，就可以使用%[]。 
　　%[]类似于一个正则表达式。[a-z]表示读取a-z的所有字符，[^a-z]表示读取除a-z以外的所有字符。 
　　所以那个问题也就迎刃而解了: 
　　sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] - %[0-9,:]", sztime1, sztime2); 
　　在softmse (Jake) 的问题贴http://community.csdn.net/Expert/topic/4843/4843294.xml?temp=.4321558中 ，周星星给出了一个很cool的sscanf用例，而后通过学习，发现sscanf真棒，现做一总结。 
　　原问题： 
　　iios/12DDWDFF@122 
　　获取/和@之间的字符串怎么做 
　　C程序里面有什么函数吗? 
　　周星星的代码： 
　　#include <stdio.h> 
　　int main() 
　　{ 
　　const char* s = "iios/12DDWDFF@122"; 
　　char buf[20]; 
　　sscanf( s, "%*[^/]/%[^@]", buf ); 
　　printf( "%s/n", buf ); 
　　return 0; 
　　} 
　　结果为:12DDWDFF 
　　sscanf与scanf类似，都是用于输入的，只是后者以屏幕(stdin)为输入源，前者以固定字符串为输入源。 
　　函数原型： 
　　int scanf( const char *format [,argument]... ); 
　　其中的format可以是一个或多个 {%[*] [width] [{h | l | I64 | L}]type | ' ' | '/t' | '/n' | 非%符号}， 
　　注：{a|b|c}表示a,b,c中选一，[d],表示可以有d也可以没有d。 
　　width:宽度，一般可以忽略，用法如： 
　　const char sourceStr[] = "hello, world"; 
　　char buf[10] = ; 
　　sscanf(sourceStr, "%5s", buf); //%5s，只取5个字符 
　　cout << buf<< endl; 
　　结果为:hello 
　　{h | l | I64 | L}:参数的size,通常h表示单字节size，I表示2字节 size,L表示4字节size(double例外),l64表示8字节size。 
　　type :这就很多了，就是%s,%d之类。 
　　特别的： 
　　%*[width] [{h | l | I64 | L}]type 表示满足该条件的被过滤掉，不会向目标参数中写入值。如： 
　　const char sourceStr[] = "hello, world"; 
　　char buf[10] = ; 
　　sscanf(sourceStr, "%*s%s", buf); //%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了 
　　cout << buf<< endl; 
　　结果为:world 
　　支持集合操作： 
　　%[a-z] 表示匹配a到z中任意字符，贪婪性(尽可能多的匹配) 
　　%[aB'] 匹配a、B、'中一员，贪婪性 
　　%[^a] 匹配非a的任意字符，贪婪性 
　　是不是感觉眼熟了啊，不错，这和正则表达式很相似，而且仍然支持过滤，即可以有%*[a-z].如： 
　　星星大哥例子回顾： 
　　const char* s = "iios/12DDWDFF@122"; 
　　char buf[20]; 
　　sscanf( s, "%*[^/]/%[^@]", buf ); 
　　printf( "%s/n", buf ); 
　　由例子3－》取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。
　　sscanf("123456 abcdedf", "%[^ ]", buf);
　　printf("%s/n", buf);
　　结果为：123456 
　　所以周星星的代码总结应该为：
　　const char* s = "iios/12DDWDFF@122";
　　char buf[20];
　　sscanf( s, "%*[^/]/%[^@]", buf );
　　printf( "%s/n", buf );
　　先将 "iios/"过滤掉，再将到字符'@'为止的一串12DDWDFF（由例3可得此串到@为止，把@122舍掉）内容即是：12DDWDFF送到buf中，得到结果。
　　PS: 向星星大哥和softmse (Jake) 致谢了，有了你们才有它啊
- 扩展阅读： 
1.[http://zhidao.baidu.com/question/37376788.html](http://zhidao.baidu.com/question/37376788.html)2.[http://www.360doc.com/showWeb/0/0/365683.aspx](http://www.360doc.com/showWeb/0/0/365683.aspx)- 开放分类： 
[c函数](http://writeblog.csdn.net/taglist?tag=c%BA%AF%CA%FD&tagfromview)
引用:http://baike.baidu.com/view/1364018.htm
