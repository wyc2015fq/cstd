# Java的char数据类型存储一个中文字符 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月06日 15:10:32[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1252








对于Java语法以及JVM框架，实际上是用之，而不是先掌握，所以对一些基础性概念总是存在偏颇认识。

比如对于这个char类型来说，一直以为是一个字节的变量，那自然不能存储一个中文字符（2个字节），这是c/c++带来的误区。




1）Java

采用Unicode编码集，一个char占用两个字节，而一个中文字符也是两个字节，因此Java中的char是可以表示一个中文字符的。



```java
public static void main(String args[]){
		char c='我';
		System.out.println(c);		
	}
```




2）c/c++

C/C++采用的字符编码集是ASCII，只有一个字节，因此是没办法表示一个中文字符的。




回顾下字符编码的三个阶段：



> 

单字节字符 – ASCII 

多字节字符 – ANSI 

宽字节字符 – UNICODE




![](http://i.imgur.com/KFA97Nj.png)







