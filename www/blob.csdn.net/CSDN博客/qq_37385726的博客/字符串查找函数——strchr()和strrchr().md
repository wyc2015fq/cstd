# 字符串查找函数——strchr()和strrchr()~ - qq_37385726的博客 - CSDN博客





2017年03月07日 16:22:52[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：156
个人分类：[C语言](https://blog.csdn.net/qq_37385726/article/category/6697679)









# C语言strchr()函数：查找某字符在字符串中首次出现的位置

<上一节下一节>- 分享到：

QQ空间
新浪微博
腾讯微博
豆瓣
人人网




[C语言中文网推出辅导班啦，包括「C语言辅导班、C++辅导班、算法/数据结构辅导班」，全部都是一对一教学：一对一辅导 + 一对一答疑 + 布置作业 + 项目实践 + 永久学习。QQ在线，随时响应！](http://fudao.biancheng.net/)



头文件：#include <string.h>


strchr() 用来查找某字符在字符串中首次出现的位置，其原型为：

    char * strchr (const char *str, int c);


【参数】str 为要查找的字符串，c 为要查找的字符。


strchr() 将会找出 str 字符串中第一次出现的字符 c 的地址，然后将该地址返回。

注意：字符串 str 的结束标志 NUL 也会被纳入检索范围，所以 str 的组后一个字符也可以被定位。


【返回值】如果找到指定的字符则返回该字符所在地址，否则返回 NULL。


返回的地址是字符串在内存中随机分配的地址再加上你所搜索的字符在字符串位置。设字符在字符串中首次出现的位置为 i，那么返回的地址可以理解为 str + i。


提示：如果希望查找某字符在字符串中最后一次出现的位置，可以使用 [strrchr()](http://c.biancheng.net/cpp/html/172.html) 函数。


【实例】查找字符5首次出现的位置。



[纯文本](http://c.biancheng.net/cpp/html/161.html#)[复制](http://c.biancheng.net/cpp/html/161.html#)
- #include<stdio.h>
- #include<stdlib.h>
- #include<string.h>
- 
- intmain(){
- char*s ="0123456789012345678901234567890";
- char*p;
-     p =strchr(s,'5');
- printf("%ld\n", s);
- printf("%ld\n", p);
- 
- system("pause");
- return0;
- }



输出结果：

12016464

12016469

==================这个结果也就说明了第一次出现这个字符是在第五个的位置~~




