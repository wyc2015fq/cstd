# jar反编译成java工具 - xqhrs232的专栏 - CSDN博客
2018年05月28日 13:00:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3555
原文地址::[https://blog.csdn.net/wzp09tjlg/article/details/68945431](https://blog.csdn.net/wzp09tjlg/article/details/68945431)
相关文章
1、[将JAR包反编译，修改后重新打包（转）](http://blog.csdn.net/junjie319/article/details/43601287)----[https://www.cnblogs.com/the-tops/p/6207191.html](https://www.cnblogs.com/the-tops/p/6207191.html)
2、修改Jar包源码(无需反编译工具)（文章看起来很长，其实方法超级简单！）----[https://blog.csdn.net/coycleipenghui/article/details/52998957](https://blog.csdn.net/coycleipenghui/article/details/52998957)
3、[将一个项目打成一个jar包，导入到另一个项目中并调用](%E5%B0%86%E4%B8%80%E4%B8%AA%E9%A1%B9%E7%9B%AE%E6%89%93%E6%88%90%E4%B8%80%E4%B8%AAjar%E5%8C%85%EF%BC%8C%E5%AF%BC%E5%85%A5%E5%88%B0%E5%8F%A6%E4%B8%80%E4%B8%AA%E9%A1%B9%E7%9B%AE%E4%B8%AD%E5%B9%B6%E8%B0%83%E7%94%A8)----[https://blog.csdn.net/eclipse_yin/article/details/51083829](https://blog.csdn.net/eclipse_yin/article/details/51083829)
4、[Eclipse JAVA项目的 目录结构 和 导入](https://www.cnblogs.com/oucbl/p/6071140.html)----[https://www.cnblogs.com/oucbl/p/6071140.html](https://www.cnblogs.com/oucbl/p/6071140.html)
在开发过程中，经常使用到jar包。有的jar是在ide不能查看源码的。
这时就可以使用到jar的反编译工具。
传送门：
http://download.csdn.net/detail/wzp09tjlg/9801433 
免费下载的一个反编译工具，解压之后可以看到
![](https://img-blog.csdn.net/20170401164401168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样几个文件。点击XJad.exe文件，打开需要反编译的jar包或者是需要反编译成java的文件夹：
![](https://img-blog.csdn.net/20170401164816138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在就可以选择是发编译jar还是反编译文件夹了。
1.反编译jar之后会在jar包的同级文件夹中生成相应的文件，里边的文件都是.java文件
反编译的之后的文件如图:
![](https://img-blog.csdn.net/20170401165223156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.反编译文件夹时 生成的.java文件就会在文件家中,会造成.class文件和.java文件同在，看起来容易让人混淆。
反编译之后的文件内容如图：
![](https://img-blog.csdn.net/20170401165259828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后,当然针对java开发的小伙伴来讲直接在IDE中就能看到jar的源码，对于其他的语言的小伙伴，如果没有编译java的ide 可以采用这样的方法来获取jar的源码.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PS.在window平台中 其实还有另外一个工具 是可以直接看jar的源码的，这个工具就是jd-gui.exe。(Mac中有没有这个工具，我不得而知.)
使用方式：下载文件 http://download.csdn.net/detail/wzp09tjlg/9801574
打开jd-gui.exe 如图：
![](https://img-blog.csdn.net/20170401170915605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择希望查看的jar包.
右边会有这个包下的所有类(当然如果有混淆，查看的是混淆之后的代码 )
![](https://img-blog.csdn.net/20170401171105201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pwMDl0amxn/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里查看的源码是没有经过混淆的，所以对于变量及命名都是可以识别的。
