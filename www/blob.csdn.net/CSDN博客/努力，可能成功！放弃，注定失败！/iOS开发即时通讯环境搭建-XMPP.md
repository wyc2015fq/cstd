# iOS开发即时通讯环境搭建-XMPP - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月11日 13:31:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2953
> 
**即时通信**是一个实时通信系统，允许两人或多人使用网络实时的传递文字，消息，文件，语音与视频交流。
## 关于XMPP
XMPP是一种基于XML的协议，它继承了在XML环境中灵活的发展性。XMPP是可扩展的。可以通过发送扩展的信息来处理用户的需求，以及在 XMPP的顶端建立如内容发布系统和基于地址的服务等应用程序。而且，XMPP包含了针对服务器端的软件协议，使之能与另一个进行通话，这使得开发者更容易建立客户应用程序或给一个配置好的系统添加功能。
## 搭建环境
###### 安装openfire软件，
**注**在此之前，我们必须确保我们的MAC已经搭建好JAVA环境。
**1.**openfire官方下载地址：[[http://www.igniterealtime.org/downloads/index.jsp#openfire](http://www.igniterealtime.org/downloads/index.jsp#openfire)]
![](http://upload-images.jianshu.io/upload_images/1230517-538a8d7fe86eb2bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
openfire软件下载
![Uploading 屏幕快照 2016-03-03 下午8.52.09_541320.png . . .]
**2.**安装完成之后我们可以在系统偏好设置下找到这个软件。
![](http://upload-images.jianshu.io/upload_images/1230517-2078b74a8b830958.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
openfire软件位置
**3.**打开软件，使其Status处于Running，如果是Stop状态需要手动开启一下。
![](http://upload-images.jianshu.io/upload_images/1230517-5731bd693f3e1e6f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
使openfire处于Running状态
**4.**然后，点击右侧的Administration栏的按钮（只有一个按钮）跳转到openfire的主页，搭建一个本地服务器。
**5.**进入网页之后选择根据语言进行选择，这里我们选择简体中文，然后continue。
![](http://upload-images.jianshu.io/upload_images/1230517-863773e4b4db63f8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置服务器
我们创建本地服务器，所以在域这个选项中写**127.0.0.1**。127.0.0.1是回送地址，指本地机，也就是本机的IP地址。然后点击继续按钮。
**6.**进行数据库设置，这个页面会提示希望如何接到Openfire数据库。这里我们根据需求，保持默认，就选择一个标准的数据库连接，而对于嵌入的数据库是为一些大型项目而设置的。设置完成点击继续就可以了。
**7.**上一步结束之后，就会提示我们对数据库进行进步的设置。这里我们使用的是MySQL数据库，所以在数据库驱动选项中选择MySQL数据库。
![](http://upload-images.jianshu.io/upload_images/1230517-99391e610cfa80db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
数据库设置
**8.**先保持浏览器的窗口，接下来我们需要，创建一个MySQL数据库。我们使用另一个软件XAMPP。
![](http://upload-images.jianshu.io/upload_images/1230517-f3f8cd20294ec7de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
XAMPP
下载地址：[[https://www.apachefriends.org/zh_cn/index.html](https://www.apachefriends.org/zh_cn/index.html)]
**9.**安装完成之后打开软件，打开之后的状态如下：
![](http://upload-images.jianshu.io/upload_images/1230517-4cf6532f5dd63a2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
XAMPP界面
**10.**点击第二个按钮Manager Servers，然后点击开启所有。开启成功之后，会亮起三个小绿灯。
![](http://upload-images.jianshu.io/upload_images/1230517-a3643b67d9c50c91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
开启服务
**11.**点击welcome返回到欢迎界面，点击右下角Go To Application，进入一个网页。
![](http://upload-images.jianshu.io/upload_images/1230517-cee228802356b41a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击左下角的phpMyAdmin选项
**12.**在phpMyAdmin创建一个新的表，点击左边的new创建一个新的表。
![](http://upload-images.jianshu.io/upload_images/1230517-8d8ddc9394276563.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建一个新的表
**13.**为数据库取一个名字，我们在这里取名为openFire，然后在左侧列表中找到我们们创建的数据库。这时候我们需要导入openfire软件的MySQL数据库。但是这个库在什么地方呢？
> 
点击Finder->按住Shift+Cmd+G->在前往文件夹这个这个对话框中输入：usr/locali->点击前往->找到openfire->这个时候我们发现无法访问->右键，显示简介->在显示简介的右下角点击解锁->找到共享与权限选项->将其全部设置为 读与写->这个时候我们就可以点击打开了->找到resources文件夹->这个文件夹下的database有许多文件->将open fire_mysql.sql拖到桌面
**14.**导入这个库
在phpMyAdmin的最上面有一个导入选项。
![](http://upload-images.jianshu.io/upload_images/1230517-f3170fa7091799ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
导入
**15.**点击选择计算机中的文件，找到刚刚拖出来的文件，导入之后，点击执行，导入文件。
![](http://upload-images.jianshu.io/upload_images/1230517-a7ad9e86afff69b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
导入本地库
**16.**导入完成的状态
![](http://upload-images.jianshu.io/upload_images/1230517-4d7e62f6a257b48d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
导入完成
**17.**进入之前openfire的网页继续往下设置，设置数据库的URL，将这个URL中的[host-name]替换成127.0.0.1,[database-name]替换成数据库名(这里我们创建的是openfire)，用户名写root，密码不用填写，点击继续。
![](http://upload-images.jianshu.io/upload_images/1230517-0a6f8f04db8af53d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
继续设置openfire
**18.**继续设置，接下来的特性设置我们保持默认，使用初始设置。
**19.**设置管理员账户，填写一个管理员邮箱，填写自己的邮箱即可，设置一个密码，要记住这个密码，我们一会使用这个密码登录我们的管理控制台。
![](http://upload-images.jianshu.io/upload_images/1230517-0a9eaaba551f9086.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置管理员账户
**20.**点击继续安装完成。
![](http://upload-images.jianshu.io/upload_images/1230517-7400646d89ea4a1b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装完成
**21.**大功告成，我们可以点击登陆到控制台了。
![](http://upload-images.jianshu.io/upload_images/1230517-6c17a0de349cacdf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
登录
![](http://upload-images.jianshu.io/upload_images/1230517-eca968fefe6fe225.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
登录到控制台
**22.**我们可以在mac系统下的信息App中使用127.0.0.1这个账户，进行验证，如果没有这个账户就自己创建一个。Cmd+，点击左下角的+，选择创建其他账户。账户类型选择Jabber,用户名设置为admin@127.0.0.1，设置密码。登录。
**23.**我们切换在线和离线状态查看管理员控制台中的**用户/组**选项下的admin的状态，注意切换之后可能需要刷新网页来查看。
## Last
到这里我们就配置好了XMPP需要的环境。接下来，我们就可以写一个基于XMPP的工程了。。。。。。
