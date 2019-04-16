# Android应用中如何保护JAVA代码 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年08月22日 14:17:44[gloomyfish](https://me.csdn.net/jia20003)阅读数：5210








Android应用中如何保护JAVA代码

Java Classes字节码的反编译太容易了，有很多功能强大的反编译利器可以轻松的将Java字节码

反转为源代码，但是android中普通.apk文件可以轻松的被反编译为Java源代码吗？

答案是当然可以，而且是相当的轻松，具体步骤如下：

1.       选择你要反编译的.apk文件， 如果它在你的Android手机里面怎么办

          参考这里 –>http://blog.csdn.net/jia20003/article/details/5293958

2.       本质上.apk文件只是zip格式的压缩文件，因此你可以用WinZip，一个比较好用的工具是7-zip

          打开.apk文件，网上可以download到7-zip

3.       网上随便找了一个android的应用，打开apk文件结构应该如下：

![](http://hi.csdn.net/attachment/201108/22/0_13139939147Hvq.gif)

          res文件里面放置都是主要包括layout，drawable等子文件夹已经里面的xml文件

          META-INF主要记录资源列表和各自对应的二十字节SHA1-Digest的Key用来保护.apk文件，

         不能通过非build修改和内容替换。最感兴趣的应该是classes.dex文件，这个就是我们要反编译的对象。

4.       把classes.dex文件copy到本地，使用google提供的开源工具dex2jar

          在windows命令行中输入如下：dex2jar.batxxx.apk 成功执行以后你就发现多了一个对应apk

          文件的jar文件

5.       然后从jar文件到java源代码，相信没有人觉得是难事了。



看到自己辛辛苦苦开发出来的Android应用如此快的被别人反编译和copy源代码，相信很多开发者要吐血了，不过只要我们稍微加点改变，想要这么轻松的读懂classes.dex文件反编译出来的文件也许就没那么容易了，基于Java的代码混淆器ProGuard已经通过了对Android中Java应用的代码保护，而且google已经把它集成在android的开发环境中，但是很多时候都被大家无视！

第一步：启用ProGuard

找到android项目**default.properties**文件在里面加入proguard.config=proguard.cfg

第二步：配置ProGuard

有时候ProGuard会根据情况remove掉一些它认为没有用的class而实际上这些class仍然被使用，这些情况包括以下几种：

     - Class只在AndroidManifest.xml文件中被引用

    - 通过JNI的方法调用

    - 通过反射动态被引用的方法和域值

当你得到ClassNotFoundException之类的错误的时候，你就应该意识到要自己配置了:

-keep public class <myclass> 更多的参数选项可以参考ProGuard官方文档。



参考引用：

[http://developer.android.com/guide/developing/tools/proguard.html](http://developer.android.com/guide/developing/tools/proguard.html)

[http://code.google.com/p/dex2jar/](http://code.google.com/p/dex2jar/)



