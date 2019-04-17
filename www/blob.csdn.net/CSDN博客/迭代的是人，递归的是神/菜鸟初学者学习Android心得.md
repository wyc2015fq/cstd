# 菜鸟初学者学习Android心得 - 迭代的是人，递归的是神 - CSDN博客





2011年08月31日 20:00:07[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：4315标签：[android																[eclipse																[java																[tools																[interface																[开发工具](https://so.csdn.net/so/search/s.do?q=开发工具&t=blog)
个人分类：[android](https://blog.csdn.net/yang_xian521/article/category/878601)





两天前，我对android的开发是一无所知的。之前有过面向pc机的vc++开发经验。我也把我摸索的过程分享给大家，希望能对其他想涉足android的朋友有所帮助，也请各路大牛对我的学习进行指导指正。

我在研究android之前只知道想开发手机程序的话可以选择android，但这东西是用java开发的，心里还是有点蹙，网上搜了如何入门的资料，感觉都介绍的不太详细。还是我自己总结一下吧

首先，当然是磨刀不误砍柴工啊，想进行android开发，肯定得有合适的工具，我这里采用的开发工具是JDK（java SDK）、eclipse3.7、android SDK,以上3个软件都能去官网免费下载到，网上的资料也基本告诉我，这3样东西就是开发的主流工具，当然，JDK的安装是为了使用eclipse。这里不能不提一下，就是为了更好的开发，我也在eclipse里安装了插件ADT（android
 development tools），ADT的安装很容易，就是在eclipse的help菜单里选择install new software，在网址中填入https://dl-ssl.google.com/android/eclipse/，然后在出现的developer tools中勾选android DDMS和android development tools两个选项，安装即可。

这里再补充几个问题：

> 
1，要将android SDK的安装路径中的tools目录添加到系统的环境变量Path中去，记得要与上一个目录之间添加；以表示分开

2，在eclipse中的window的preferences中的android选项卡中将android的安装路径添加进去


进行了上述两步后启动ADT时我还遇到启动错误，你可能不会遇到，（我的系统文件夹的位置被我改变过），我又将.Android这个文件夹复制到错误提示的文件夹中解决第一个问题，再次启动，又遇到错误：

invalid command-line parameter: Files\Android\android-sdk\tools/emulator-arm.exe.

Hint: use ‘@foo’ to launch a virtual device named ‘foo’.

please use -help for more information

是由于我的安装目录中有Program Files的空格造成的，如果不想更改android sdk的安装目录，那么可以将eclipse中window的preferences中的android设置的路径改为

C:\Progra~1\Android\android-sdk-windows，（只是参考路径）、

修改完毕后，终于可以享用android模拟器啦！~~

软件都安装好了，我想下一步就是对java的学习了，我这里考虑自己有c++的基础，有比较好的对面向对象的程序设计的理解，就随便找了本java的入门教材，考虑自己英文不好，没选择老外写的《think in java》，选了本《JAVA2实用教程》（清华出版社），感觉非常一般的一本教材，还好我只是粗略的看了下语法，这里把我认为java里一些我不清楚的语法也分享给大家，让大家见笑了，

> 
1，java中队成员变量的操作只能在方法（就是c语言中的函数，我理解）中，但在定义类的成员变量时刻赋初值


> 
2，创建对象的两个步骤：


> 
> 
1）对象声明



> 
> 
> 
类名字 对象名字；


2）对象分配内存



> 
> 
> 
对象名 = new 类名（）；



3，方法被调用时，参数传递都是形参的传递

4，1）实例变量：前面无static前缀，不共享这个类变量；

      2）类变量：前有static前缀，所有对象的这个类变量共享；

5，1）实例方法：前面无static前缀，即可操作实例变量，又可操作类变量；

      2）类方法：前有static前缀，入口地址被所有对象共享；

6，this表示某个对象，可出现在实例方法、构造方法中，不可出现在类方法

7，不用public、private、protected修饰的变量、方法称为友好变量、方法。同一个包（package）内，可以通过类名访问其他类的友好成员

8，子类的声明方法：class 子类名 extends 父类名

9，final类不能被继承，表示不能轻易修改的意思，是一个关键字

10，abstract类可以有abstract方法。abstract方法：只有声明，没有实现（有点像c++里的虚函数）

11，子类项用呗子类隐藏（覆盖）的成员变量、方法，用关键字super

12，接口的声明：


> 
> 


interface 接口名；

接口体中只进行声明，不提供方法的实现


        类调用接口


> 
> 
class 类名 implement 接口名；



我对java只是从语法上理解了一下，对java语言博大精深的内涵理解还差好多，这里没法总结出什么更精彩的东西。

以上就是我这两天来学习android的总结，这两天要抓紧学习了，希望自己没走太多弯路，哪位大侠路过多给点指点，小弟不胜感激](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




