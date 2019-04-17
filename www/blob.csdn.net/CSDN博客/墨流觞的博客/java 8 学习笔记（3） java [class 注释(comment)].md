# java 8 学习笔记（3） java [class 注释(comment)] - 墨流觞的博客 - CSDN博客





2018年09月25日 23:03:08[墨氲](https://me.csdn.net/dss_dssssd)阅读数：121








### 注释

```java
```java
// 单行注释
   
   /*
   多行注释
   */
  
   
   /**
   文档注释
   */
```
```

### class

![在这里插入图片描述](https://img-blog.csdn.net/20180925225056520?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

每个类在java中有两个名字：
- 简单的名字
- 一个完全限定名（ a fully qualified name）

```java
```java
// Welcome.java
package com.jdojo.intro;
class Welcome {
public static void main(String[] args) {
System.out.println("Welcome to the Java world.");
}
```
```

类的简单名字就是在类定义是出现先在关键字,比如Welcome,而完全限定名就是软件包+类的简单名字。比如 com.jdojo.intro.Welcome



