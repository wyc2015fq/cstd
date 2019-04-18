# 含位域结构体的sizeof  - ljx0305的专栏 - CSDN博客
2008年05月08日 23:30:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：604标签：[struct																[存储																[编译器																[测试																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
使用位域的主要目的是压缩存储，其大致规则为：
1) 如果相邻位域字段的类型相同，且其位宽之和小于类型的sizeof大小，则后面的字段将紧邻前一个字段存储，直到不能容纳为止；
2) 如果相邻位域字段的类型相同，但其位宽之和大于类型的sizeof大小，则后面的字段将从新的存储单元开始，其偏移量为其类型大小的整数倍；
3) 如果相邻的位域字段的类型不同，则各编译器的具体实现有差异，VC6采取不压缩方式，Dev-C++采取压缩方式；
4) 如果位域字段之间穿插着非位域字段，则不进行压缩；
5) 整个结构体的总大小为最宽基本类型成员大小的整数倍。
测试:
struct test
 {
  char a:1;
  char :2;
  long b:3;
  char c:2;
 };
 test t1;
 int len=sizeof(t1);   //len=12
struct test
 {
  char a:1;
  char :2;
  char b:3;
  long c:2;
 };
 test t1;
 int len=sizeof(t1);   //len=8
struct test
 {
  char a:1;
  char :2;
  char b:3;
  char c:2;
 };
 test t1;
 int len=sizeof(t1);   //len=1
转自:http://www.cnitblog.com/wuzhimeili/archive/2007/01/17/21963.html
