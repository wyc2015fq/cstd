# IIS发布中遇到的问题 - Big Smile - CSDN博客
2016年04月14日 20:54:46[王啸tr1912](https://me.csdn.net/tr1912)阅读数：3839
这两天发布牛腩和人事的维护系统，在IIS上遇到了很多的问题，在这里和大家分享一下：
# 一、Web 服务器被配置为不列出此目录的内容。
出现这个问题其实解决起来异常的简单，就是在配置文件里面没有写明默认要执行的第一个文件是哪个，一般出现在网址这样输入的时候：http://192.168.24.36:8020/  没有指定文件，如果这样就会好：http://192.168.24.36:8020/Default.aspx  
![](https://img-blog.csdn.net/20160414195002317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160414195136959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          那么，用户是不知道要打开哪个文件的，这就需要我们自己进行设置：
在配置文件中写入
```
<system.webServer>
        <defaultDocument>
            <files>
                <clear />
               <add value="Default.aspx" />
            </files>
        </defaultDocument>
    </system.webServer>
```
或者直接在vs中点击：
![](https://img-blog.csdn.net/20160414194919613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 效果是一样的。
第三种方法就是在IIS中的网站中默认文档处添加该文件名：
![](https://img-blog.csdn.net/20160414195418643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160414195440549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        一般情况下，如果是第一发布的系统，就在iis上填写即可，如上图，因为他还要在下面填写上默认的地址，所以如果配置文件和这里有冲突的话可能会报错。这也可以比较灵活的应用系统（其实这里修改的也是配置文件）。
# **二、主机名称的问题**
        如果你在建立网站的时候在主机名称中填写了自定义的名字，那么恭喜你想要有自己的域名了，但是在DNS服务器未认同的情况下你是不会访问到你需要访问的网站的。主机名称就是你在网上购买到的，或者是在DNS服务器上已经有了的域名，一般自己是无法自定义的，如果自定义了的话，就会出现：
![](https://img-blog.csdn.net/20160414203048219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这样的页面，跟没联网是的。
但是如果非要连接的话办法还是有的，就是修改要打开网站的计算机host文件，把这个域名指定为发布的服务器的ip，然后就可以了，具体可以访问：C:\Windows\System32\drivers\etc的hosts文件 然后添加域名和ip的对应项就可以访问了如：
           tt.com     127.0.0.1           只不过访问限制比较厉害。
# 三、文件权限的问题
有的时候即使是都设置好了，也会 出现网站不能访问的问题，例如权限不足，或网页出错，或者跳转的时候不可以更改等错误，这个时候我们就需要提权了。
如图，需要把EveryOne的权限加入，并且给所有的权限。
![](https://img-blog.csdn.net/20160414204125745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 四、程序池的设置问题
         首先什么是IIS的应用程序池呢？百度的解释：应用程序池是将一个或多个应用程序链接到一个或多个工作进程集合的配置。因为应用程序池中的应用程序与其他应用程序被工作进程边界分隔，所以某个应用程序池中的应用程序不会受到其他应用程序池中应用程序所产生的问题的影响。
        所以每个网站基本上都有一个程序池来对应，还有多个网站对应一个程序池的情况。这个程序池的设置也是很重要的：
![](https://img-blog.csdn.net/20160414204607829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       首先就是.NET版本的设置，根据自己的网站程序选择相应的.net版本，能够正常的发布，还有托管管道的模式，这一点是和网站程序的web.config文件有关的，老版本的配置文件是不能用在新的集成模式的管道模式中的具体看博客：[http://blog.csdn.net/darren__chan/article/details/8215646](http://blog.csdn.net/darren__chan/article/details/8215646)
另外在某些32位网站发布的时候还需要注意32位的支持问题，在程序池的高级设置中有这样的设置：
![](https://img-blog.csdn.net/20160414205128947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)把相应的地方设置为true即可。如果不这样设置的话会有SQLite的错误，如图：
![](https://img-blog.csdn.net/20160414205356278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         以上为这几天遇到的问题，还望读者们多多指点。
