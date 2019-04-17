# Windows下Java JDK的安装与配置 - 小灰笔记 - CSDN博客





2017年02月21日 22:43:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：214








在Mac上折腾了一段时间了，感觉还不错。前几天手痒买了ThinkPad，这几天再感受一下ThinkPad以及Windows。




很多人以博客或者其他的方式比较Linux或者UNIX与Windows的软件安装难易程度以及方便程度，最终得出的结论一般都是前者要好一些。其实，个人倒感觉写这类文章的多半是前者的用户，而那么沉默不发言的一般都是纯粹的Windows还是用者。我个人是UNIX的爱好者，也确实是感觉到不管是Mac还是Linux，都有其独到之处。不过，Windows道也算不上那么让人纠结，简单的双击以及一路默认基本上能够搞定我们的所有需求。




今天尝试安装Java的JDK，其实也是简单的双击。基于我自己的喜好修改了一下安装的位置。安装完成，接下来就是涉及到的配置问题了，需要修改以下环境变量，确切说是增加以下环境变量信息：







CLASSPATH=.;D:\software\Java\jdk1.8.0_121\lib



JAVA_HOME=D:\software\Java



Path=D:\software\Java\jdk1.8.0_121\bin





接下来测试一下安装好的编译器以及解析器是否可用：

![](https://img-blog.csdn.net/20170221224940768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170221225003683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





通过以上简单的测试，编译器以及解析器都没有问题了。接下来写个最简单的程序测试一下，代码如下：

public class HelloWorld{
public static void main(String[] args){
System.out.println("hello");
}

}





编译以及运行的结果如下;

![](https://img-blog.csdn.net/20170221225245140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这样看来，应该说整个开发环境最基本的东西已经具备了。



