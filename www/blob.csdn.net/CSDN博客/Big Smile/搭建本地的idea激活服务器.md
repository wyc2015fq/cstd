# 搭建本地的idea激活服务器 - Big Smile - CSDN博客
2017年12月28日 22:30:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：5342
# 前言
        博主用的是idea这个IDE，因为最近idea官方打击第三方激活服务有些严重，所以我的idea经常处于今天可以用，到了明天就不能用的状态，所以，从idea激活的网站找到了本地的idea激活服务，和大家分享一下。
# 一、下载服务包
        首先，需要在[http://idea.lanyus.com/](http://idea.lanyus.com/)这个网站下面下载一个idea的本地服务器，本人呢也上传了一个到csdn上，地址：链接：[https://pan.baidu.com/s/1boCHwZl](https://pan.baidu.com/s/1boCHwZl) 密码：yjqd
# 二、安装
         说是安装了，其实直接解压就可以了，在一个你认为可以直接放这个服务的地方直接解压缩，然后运行系统位数对应的exe程序就可以了。
![](https://img-blog.csdn.net/20171228224437341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     然后我们就可以在idea的激活页面就可以直接填写我们创建的这个服务。
![](https://img-blog.csdn.net/20171228224541775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20171228224820123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样就可以了，我们就可以正常使用我们的开发IDE了。
        第一次我们为了可以看到激活地址，可以显示这个黑框，但是只要我们重启了电脑之后，这个服务就不运行了，所以我们在次启动IDEA的时候，还是需要输入激活地址，并启动这个程序，那么我们要怎么办呢？
       这个时候，我们要把这个exe程序创建成windows的服务，就可以让他在开机的时候自动在后台运行了，具体操作是：
     以管理员身份打开cmd命令提示符（实测powershell不行），然后输入：
sc create IdeaRegistServer binpath= D:\tools\IdeaRegistServer\IntelliJIDEALicenseServer_windows_amd64.exe type= own start= auto displayname= Regist_Server
      然后我们在service.msc中就可以看到这个服务了，我们可以设置这个服务开机自动启动，然后我们就可以畅想激活的乐趣了，再也不用担心被封杀了，哈哈哈。
**更新，更新，更新！重要的事情说三遍！**
上面忘了说一点了，就是这个激活服务也不是一劳永逸的，也是有到期时间的，至2018年10月14日，这个日期一过，就又需要重新找激活了，所以我们还是找破解补丁的方式激活的，在我最新的博客中将会带大家探索。
关于设置自动启动服务，由于上述程序不是很标准的windows服务程序，所以有的版本的系统启动会有很多问题，最终的解决方案是写个vbs脚本，放到启动文件夹里面，然后就可以实现激活服务后台运行了，附上代码：
```
Dim WShell
Set WShell = CreateObject("WScript.Shell")
WShell.Run "D:\tools\IdeaRegistServer\IntelliJIDEALicenseServer_windows_amd64.exe", 0     '后面0的意思是“隐藏”
Set WShell = Nothing
```
引号中的是激活exe放置的目录，替换成自己的就行，把上述代码放到文本文档中，保存后修改扩展名为.vbs即可运行了。
统一说一下，最最新版的idea已经不支持这种服务器激活了，希望大家改用jar包激活方式，缺点是idea更新的时候会问是否回滚我们修改过的两个文件，只要跳过就好了。
[https://blog.csdn.net/halen001/article/details/81137092](https://blog.csdn.net/halen001/article/details/81137092)
