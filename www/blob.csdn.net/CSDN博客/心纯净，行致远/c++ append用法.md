# c++ append用法 - 心纯净，行致远 - CSDN博客





2018年12月02日 20:15:36[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：123








append函数是向string的后面追加字符或字符串。头文件是<string>.


**1).向string的后面加C-string**

string s = “hello “; const char *c = “out here “;

s.append(c); // 把c类型字符串s连接到当前字符串结尾

s = “hello out here”;


**2).向string的后面加C-string的一部分**

string s=”hello “；const char *c = “out here “;

s.append(c,3); // 把c类型字符串s的前n个字符连接到当前字符串结尾

s = “hello out”;


**3).向string的后面加string**

string s1 = “hello “; string s2 = “wide “; string s3 = “world “;

s1.append(s2); s1 += s3; //把字符串s连接到当前字符串的结尾

s1 = “hello wide “; s1 = “hello wide world “;


**4).向string的后面加string的一部分**

string s1 = “hello “, s2 = “wide world “;

s1.append(s2, 5, 5); ////把字符串s2中从5开始的5个字符连接到当前字符串的结尾

s1 = “hello world”;

string str1 = “hello “, str2 = “wide world “;

str1.append(str2.begin()+5, str2.end()); //把s2的迭代器begin()+5和end()之间的部分连接到当前字符串的结尾

str1 = “hello world”;


**5).向string后面加多个字符**

string s1 = “hello “;

s1.append(4,’!’); //在当前字符串结尾添加4个字符!

s1 = “hello !!!!”;

---------------------

作者：wxn704414736

来源：CSDN

原文：https://blog.csdn.net/wxn704414736/article/details/78551886

版权声明：本文为博主原创文章，转载请附上博文链接！



