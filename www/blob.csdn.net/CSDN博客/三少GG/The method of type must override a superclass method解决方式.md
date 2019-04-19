# The method of type must override a superclass method解决方式 - 三少GG - CSDN博客
2010年09月28日 23:19:00[三少GG](https://me.csdn.net/scut1135)阅读数：26162

如果你用的是myeclipse 6.0 而且 在使用 
@Override 时出现以下错误： 
The method XXXXXX   of type XXXXXXXXX must 
override a superclass method 
那是因为你的Compiler 是jdk1.5，只要把它改为 1.6就可以了 
方法： 
1. window ->preferences... -> java -> Compiler 
2. Compiler compliance level : 6.0 
3.Apply 
4. ok 
5.完成 
The method of type must override a superclass method解决方式
2010年08月16日 星期一 11:17
The method of type must override a superclass method，提示的是实现类必须实现接口的方法。
原因：
实现类里面使用了 @Override
在1.5下要使用@Override 这个annotation 必须保证 被标注方法来源于class 而不是interface
解决方法：
将项目的jdk从1.5改为1.6
结果：
问题解决
