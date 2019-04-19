# Windows7安装PADS2007详细步骤____亲自实验总结 - xqhrs232的专栏 - CSDN博客
2016年03月23日 22:36:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2417
原文地址:[http://jingyan.baidu.com/article/fa4125acd218ed28ac709228.html](http://jingyan.baidu.com/article/fa4125acd218ed28ac709228.html)
相关文章
1、**WIN7下安装PADS2007！备忘（折腾了3天啊）----[http://www.amobbs.com/thread-5382416-1-1.html](http://www.amobbs.com/thread-5382416-1-1.html)**
2、pads2007在win7下安装的问题，跪求答案----[http://iask.sina.com.cn/b/19029815.html](http://iask.sina.com.cn/b/19029815.html)
3、教你如何在win7系统里面安装pads2007(图文解说)----[http://www.doc88.com/p-479426143946.html](http://www.doc88.com/p-479426143946.html)
4、win7安装pads2007时遇到的问题----[http://www.eefocus.com/sbogwxf230/blog/11-10/232427_ff482.html](http://www.eefocus.com/sbogwxf230/blog/11-10/232427_ff482.html)
5、win7安装pads2007时遇到的问题----[http://blog.163.com/donga_a/blog/static/14054292013108113545313/](http://blog.163.com/donga_a/blog/static/14054292013108113545313/)
6、**PADS安装的疯了----[http://bbs.21ic.com/icview-254965-1-1.html](http://bbs.21ic.com/icview-254965-1-1.html)**
7、[装PADS200X完后,启动软件（如logic等）时提示：Security
 system wasn't properly initialized: License request for pwrshell feature failed.](http://blog.csdn.net/einsteinz/article/details/6083651)----[http://blog.csdn.net/einsteinz/article/details/6083651](http://blog.csdn.net/einsteinz/article/details/6083651)

//===============================================================================================
备注：
1〉今天在 WIN7 旗舰版上安装PADS2007报错了，安装不了！
2>报错情况如下
Error creating start menu entry for Mentor Install
Error creating start menu entry for Mentor UnInstall
3〉改变文件的兼容性运行就好----兼容XP SP3
![](https://img-blog.csdn.net/20160413094330970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4〉安装到一半50%老是重新启动----重启后又找不到安装的源文件----将ISO镜像中的文件全部COPY到硬盘再重新安装
5〉现在的情况是安装成功了，但破解不了，只能运行在demo模式
6〉要查用MentorKG.exe工具软件产生的LICENSE.TXT中的hostid是不是跟你的网卡的MAC地址是对应的
    //1111----HOSTID=5cf9dd50f4c4
   //2222----cmd----ipconfig/all
7〉按装到50%卡住，强制关进程的方法也不行----装完后缺少各种DLL，运行不起来！！！
8〉装了好多遍还是成功不了，WIN7老是装到50%就会卡住要求重启。装了好多遍不知道会不会因为注册表没清干净使得后面再装也成功不了？？？
9〉看来WIN7下只能用DEMO模式的PADS2007了！！！
