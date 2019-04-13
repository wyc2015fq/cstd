
# java的StringBuffer可变字符串 和String的区别 - 李昆鹏的博客 - CSDN博客


2018年03月16日 13:00:01[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：113


**-----------java的StringBuffer可变字符串 和String的区别----------**
**StringBuffer****是****java.lang****下的类，不能被继承**
**Java.lang****下的类是不需要引入的**
**可变字符串：**
**StringBuffer****概述****+**
**StringBuffer:****字符串缓冲区。**
**StringBuffer****和****String****区别呢？**
**String****一旦被创建后，值不能改变，如果参与了操作，引用发生变化，不是在原有的字符串上操作，而是新产生了一个字符串。**
**StringBuffer****如果发生了变化，就是在原有的字符串上发生了改变，不会创建新的字符串，而且****StringBuffer****是线程安全的。**
String s = “hello”;
S + = “world”;
System.out.println(s)//输出的是helloworld
StringBuffer：是线程安全的可变字符串。
StringBuffer b = “hello”;
B += “world”;//使用hello的空间追加world，不是新创建一个字符串。
System.out.println(s)//输出的是helloworld
StringBuffer线程安全图：
![](//img-blog.csdn.net/20180316125932943?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
例子：
**public****class**Test1 {
**public****static****void**main(String[]args) {
//创建可变字符串
StringBuffersb=**new**StringBuffer();
//把可变字符串做追加
StringBuffersb1=sb.append(**true**);
//判断追加后的字符串地址
System.**out**.println(sb==sb1);
Stringstr="abc";
//字符串追加任何类型返回的都是字符串
Stringstr1=str+**true**;
System.**out**.println(str1);
//String字符串地址比较
System.**out**.println(str==str1);
}
}
结果：
true
abctrue
false


