# Eclipse里启动Tomcat，配置内存大小 - z69183787的专栏 - CSDN博客
2014年03月22日 20:25:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：610
一般安装完eclipse之后，在安装目录下你应该可以看到有一个 eclipse.ini 文件，对了，就是在这个文件里面修改，我打开同事机器上这个文件，里面的内容是：
-vmargs
-Dosgi.splashLocation=d:\Program Files\MyEclipse 5.5.1 GA\eclipse\MyEclipseSplash.bmp
-Dosgi.splashLocation=d:\Program Files\MyEclipse 5.1.1 GA\eclipse\MyEclipseSplash.bmp
-Xms64M 
-Xmx128M
呵呵，2G内存的机器，给eclipse这么点内存。。。介绍下参数的意思：
-vmargs：说明后面是VM的参数
-Dosgi.splashLocation：表示启动eclipse时你看到的界面（不知道他这里面怎么冒出了二句相同的）
-Xms64M ：虚拟机占用系统的最小内存
-Xmx128M：虚拟机占用系统的最大内存
最后我把最大最小内存设置了下：
设置为：
-Xms128M 
-Xmx512M
够用了，呵呵。
还可以在这里面加一些参数，如：
-XX:PermSize：最小堆大小。一般报内存不足时,都是说这个太小,
                       堆空间剩余小于5%就会警告,建议把这个稍微设大一点,视自己机器内存大小来设置
-XX:MaxPermSize：最大堆大小。这个也适当大些
-Xmx512M的5%为25.6M，理论上要求-Xmx的数值与-XX:MaxPermSize必须大于25.6M
![](https://p-blog.csdn.net/images/p_blog_csdn_net/yjq8116/EntryImages/20090326/2009-02-20_164339.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/yjq8116/EntryImages/20090326/2009-02-20_164511.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/yjq8116/EntryImages/20090326/2009-03-11_135032.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/yjq8116/EntryImages/20090326/2009-03-11_135121.jpg)
[](http://photo.blog.sina.com.cn/showpic.html#blogid=5f9d7ec40100dpjs&url=http://static2.photo.sina.com.cn/orignal/5f9d7ec4t6c092e230261&690)
一般都是内存配置问题， 
可以在Eclipse快捷上点右键，选属性，在目标中文字后加入一串参数: 
-vmargs   -Xms128M   -Xmx256M   -XX:PermSize=64M   -XX:MaxPermSize=512M
解决办法：快捷图标上-属性-目标后加上：-XX:PermSize=64m -XX:MaxPermSize=128m
　重启动eclipse,搞定！
<Eclipse_Home>下面有一个eclipse.ini文件可以调整Eclipse最大使用内存，默认如下
-vmargs
-Xms40m
-Xmx256m
调整为：
-vmargs
-Xms40m
-Xmx256m
-XX:PermSize=64M
-XX:MaxPermSize=128M
1g内存推荐为：
-vmargs
-Xms128M
-Xmx512M
-XX:PermSize=64M
-XX:MaxPermSize=128M
对于MyEclipse的话，直接修改MyEclipse在桌面的快捷方式的参数就可以啦。现在解释这些参数的意思。
-vmargs：说明后面是VM的参数
-Xms40m：虚拟机占用系统的最小内存
-Xmx256m：虚拟机占用系统的最大内存
-XX:PermSize：最小堆大小。一般报内存不足时,都是说这个太小,
                       堆空间剩余小于5%就会警告,建议把这个稍微设
                               大一点,不过要视自己机器内存大小来设置
-XX:MaxPermSize：最大堆大小。这个也适当大些
-Xmx512M的5%为25.6M，理论上要求-Xmx的数值与-XX:MaxPermSize必须大于25.6M
把里面的参数改为
-vmargs 
-Xms128M 
-Xmx512M 
-XX:PermSize=128M 
-XX:MaxPermSize=256M
所以对于我而言，我只要把我的MyEclipse5.0改-XX:PermSize=128M-XX:MaxPermSize=256M，问题解决！

**eclipse 中配置jvm,  关键是在配置jvm，jvm是根源![](https://img-my.csdn.net/uploads/201301/04/1357287760_6625.jpg)**
