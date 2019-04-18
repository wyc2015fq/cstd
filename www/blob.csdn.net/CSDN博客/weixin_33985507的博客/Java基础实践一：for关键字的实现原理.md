# Java基础实践一：for关键字的实现原理 - weixin_33985507的博客 - CSDN博客
2017年05月19日 11:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Java源码：
```
/**
 * Demo.java
 * com.yuanchuangyun.libra.web
 *
 *
 *   ver     date              author
 * ──────────────────────────────────
 *            2017年5月19日         renguoqiang
 *
 * Copyright (c) 2017, yuanchuangyun All Rights Reserved.
*/
package com.yuanchuangyun.libra.web;
public class Demo {
    public void test(){
        for(int i= 0;i<5;i++){
            System.out.println(i);
        }
    }
}
```
Java反编译命令：
```
javap -c Demo.class >Demo.txt
```
Java反编译结果：
```
Compiled from "Demo.java"
public class com.yuanchuangyun.libra.web.Demo {
  public com.yuanchuangyun.libra.web.Demo();
    Code:
       0: aload_0       
       1: invokespecial #8                  // Method java/lang/Object."<init>":()V
       4: return        
  public void test();
    Code:
       0: iconst_0      
       1: istore_1      
       ***2: goto          15***
       5: getstatic     #15                 // Field java/lang/System.out:Ljava/io/PrintStream;
       8: iload_1       
       9: invokevirtual #21                 // Method java/io/PrintStream.println:(I)V
      12: iinc          1, 1
      15: iload_1       
      16: iconst_5      
      17: if_icmplt     5
      20: return        
}
```
Java里的for其实是用goto关键字实现的。
让我想起了09年一次去微软亚洲研究院面试，面试者问我C#里的for循环底层使用什么实现的，我确实不知道。
另外，11年在微软亚洲研究院因为工作需要，反编译Windows HPC Server的某个dll（C#编写的），发现里边竟然用goto关键字写代码。
因为没有学过汇编语言，难道汇编里只有goto吗？
学习这事，真是明白了一句话，要有一颗勇敢的心。
