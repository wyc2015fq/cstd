# 一个ActiveSync无法连接的解决方法 - xqhrs232的专栏 - CSDN博客
2013年10月31日 17:46:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1527
原文地址::[http://blog.csdn.net/renpine/article/details/4709437](http://blog.csdn.net/renpine/article/details/4709437)

上一次安装华为的3G驱动过后致使ActiveSync4.5不能同步Ce，表现是已经连接但是浏览里面：空文件夹。
郁闷之余只好重装系统，重装过后可以同步了，但后来不知为何又不行了，还表现出：
“无法验证设备上ActiveSync的版本，可能有程序（例如防火墙）正在阻止使用ActiveSync端口连接到该设备”
很郁闷啊。在网上搜索只有魅族论坛里面有人碰到过，但都没解决的办法。
老外也会碰到这个问题吧，一搜索果真如此还有解决方法，具体如下：
1、连接设备到PC，弹出：
![image](https://p-blog.csdn.net/images/p_blog_csdn_net/renpine/595389/o_image_thumb.png)
2、打开任务管理器，结束进程rapimgr.exe
3、从任务管理器的菜单：文件->新建任务->找到AcriveSync的安装目录，选择里面的rapimgr.exe
4、断开设备，之后重新连接到PC，即可看到文件夹内容了。
郁闷的微软的Bug。。。。。。。。。。。。。。。。。。。。
//==========================================================================
备柱::
1》在任务管理器里面把wcescomm.exe++++rapimgr.exe杀掉，再重新新建任务到C:\Program Files\Microsoft ActiveSync目录下把对应的EXE跑起来，再插拔一下USB线就可以连上了----自己亲自试了一下，还真的很容易成功的-----总算不用每次重启PC机了！！！
