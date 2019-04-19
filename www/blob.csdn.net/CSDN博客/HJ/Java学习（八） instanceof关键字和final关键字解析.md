# Java学习（八） instanceof关键字和final关键字解析 - HJ - CSDN博客
2018年02月08日 15:20:10[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：113
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、instanceof关键字
 1.定义：
①属于比较运算符；
②用来判断一个对象是否是指定类的对象；
③用法：对象 instanceof 类，该表达式是一个比较运算符，返回的结果是boolean类型 true|false；
2.注意：使用instanceof关键字做判断时，两个类之间必须有联系。
代码样例：
```java
Person p=newPerson();
System.out.println(p instanceof Person);
```
二、final关键字
1.应用：final关键字主要用于修饰类、类成员、方法、以及方法的形参；
2.说明：
①使用类名，成员。修改该成员的值会报错（因为常量不能被修改），常量名必须大写；
②final使基本数据类型值不变；使对象引用的引用恒定不变，无法让其指向一个新的对象，但是对象自身可以被修改；
③一般和static关键字结合使用，final和static可以互换位置；
④final修饰类，那么该类是最终类，不能被继承；注意，String类、Integer都是final的；（原因：为了防止代码功能被重写、该类没有必要进行扩展）；
⑤final修饰方法，该方法是最终方法，不能被重写；当一个类中的函数都被修饰为final时，可以将类定义为final的；
⑥final修饰形参，该形参所属的方法中不能被篡改；在项目中主要用于一些只用来遍历未知数据的函数，将未知变量声明为final的，增强数据的安全性；
