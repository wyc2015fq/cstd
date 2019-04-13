
# java字符串String概述、常用构造器和面试题 - 李昆鹏的博客 - CSDN博客


2018年03月15日 13:50:03[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：164


-----------------------------**java字符串概述、常用构造器和面试题**--------------------
**数据类型**
**1.****基本数据类型**
**a.****数值型**
**(1)****整数类型（****byte****，****short****，****int****，****long****）**
**(2)****浮点类型（****float****，****double****）**
**b.****字符型（****char****）**
**c.****布尔类型（****boolean****）**
**2.****引用数据类型**
**a.****类（****class****）**
**b.****接口（****interface****）**
**c.****数组****([])**
**字符串的概述**
**public****class**test1 {
**public****static****void**main(String[]args) {
//定义一个字符串的变量，赋值"abc"
Stringstr="abc";
str=str+"de";
System.**out**.println(str);
}
![](//img-blog.csdn.net/20180315134705399?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**在定义****String str =****“****abc****”的时候。“****abc****”是常量存储数据共享区，定义时先会到数据共享区中去查找是否存在“****abc****”这样的字符串，如果存**
**储在的直接把“****abc****”的地址赋值给****str****，如果数据共享区中不存在“****abc****”那么就去创建一个“****abc****”。**
**常量字符串的特点：**
**不能改变**
**String****类中，重写了****Object****的****toString****方法****.**
**--------------------****字符串的构造器****---------------------------**
**String****（）**
**初始化一个新创建的****String****对象，使其表示一个空字符序列****.**
**String****（****byte[]  bytes****）**
**通过使用平台的默认字符集解码指定的****byte****数组，构造一个新的****String****。**
**String(byte[]  byte, int offset, int  length)**
**通过使用平台的默认字符集解码指定****byte****子数组，构造一个新的****String****。**
**String****（****char[]  value****）**
**分配一个新的****String****，使其表示字符数组参数中当前包含的字符序列。**
**String(char[]  value, int offset, int  count)**
**分配一个新的****String****，它包含取字符数组参数一个字数组的字符。**
**String(String  original)**
**初始化一个新创建的****String****对象，使其表示一个与参数相同的字符**
**序列：换句话说，新创建的字符串是该参数字符串的副本。**
**例子：**
**public****class**test2 {
**public****static****void**main(String[]args) {
//创建一个空字符串的对象；
Stringstr=**new**String();
System.**out**.println("打印字符串str："+str+"-----");
//定义一个字节数组
**byte**[]bs={97,98,99,100,101,102,103};
//以字节数组作为参数的字符串对象的创建
Stringstr1=**new**String(bs);
//把字节数组中的每一个数值转换成相对应的ascii的字符后组装成字符串
System.**out**.println("以字节数组作为参数的字符串对象的创建："+str1);
//第一个参数字节数组，第二个参数是从指定的索引开始，第三个参数截取长度
//注意数组长度越界
Stringstr2=**new**String(bs, 3, 4);
System.**out**.println("以字节数组作为参数的字符串截取创建："+str2);
//创建一个字符数组
**char**[]cs= {'a','b','g','t','o'};
Stringstr3=**new**String(cs);
System.**out**.println("以字符数组作为参数创建字符串:"+str3);
//第一个参数字符数组，第二个参数是从指定的索引开始，第三个参数截取长度
//注意数组长度越界
Stringstr4=**new**String(cs,3,2);
System.**out**.println("以字符数组作为参数的字符串截取创建："+str4);
//以常量的字符串类作为参数
Stringstr5=**new**String("qwe");
System.**out**.println("以常量字符串作为参数："+str5);
}
}
打印结果：
打印字符串str：-----
以字节数组作为参数的字符串对象的创建：abcdefg
以字节数组作为参数的字符串截取创建：defg
以字符数组作为参数创建字符串:abgto
以字符数组作为参数的字符串截取创建：to
以常量字符串作为参数：qwe
-------------------------**面试题**-----------------------------
**public****class**test3 {
**public****static****void**main(String[]args) {
//创建常量的字符串
Stringstr="helloworld";
//创建字符串的对象
Stringstr1=**new**String("helloworld");
//两个字符串地址的比较
System.**out**.println("两个字符串地址的比较:"+ (str==str1));
//两个字符串值的比较
System.**out**.println("两个字符串值的比较:"+str.equals(str1));
System.**out**.println("-----------------------------");
//定义一个字符串的常量
Stringstr2="renliang";
Stringstr3="renliang";
//判断两个字符串地址是否相等
System.**out**.println("判断两个字符串地址是否相等:"+ (str2==str3));
System.**out**.println("-----------------------------");
Stringstr4="hello";
Stringstr5="world";
Stringstr6="helloworld";
//str4和str5拼接后一定会产生新的字符串
//即使在数据共享区中存储拼接后的值相等的字符串，也会产生新的地址
System.**out**.println(str6== (str4+str5));
//如果是两个没有引用的常量做拼接那么就会去数据共享区中查找是否有相等的字符串
//如果有相等的就不去创建新的字符串了，直接使用
System.**out**.println(str6== ("hello"+"world"));
}
}
**结果：**
![](//img-blog.csdn.net/20180315134738991?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**内存图：**
![](//img-blog.csdn.net/20180315134807667?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**String str =****“****hellworld****”在数据共享区创建的是一个对象。**
**String str1 = new String****（“****helloworld****”）**
**在堆中创建一个字符串的对象，然后再到数据共享区中创建一个字符串的常量的对象，然后把堆中的对象指向数据共享区中的对象。如果数据共享区有相同的字符串对象，则值需在堆中创建一个对象，然后****指向****数据共享区中相同的常量字符串。**


