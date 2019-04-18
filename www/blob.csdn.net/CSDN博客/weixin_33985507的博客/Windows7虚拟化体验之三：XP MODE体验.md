# Windows7虚拟化体验之三：XP MODE体验 - weixin_33985507的博客 - CSDN博客
2009年07月14日 10:49:20[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
XP MODE体验
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
部署完XP MODE后，我们可以先来简单地体验一下在Windows 7下运行XP程序的效果。如图1所示，在Windows7的程序组中找到Windows Virtial PC下的Virtual Windows XP，这个Virtual Windows XP其实就是XP MODE。只要执行Virtual Windows XP，Windows Virtual PC就会自动载入XP MODE虚拟机。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539622160.jpg)<?xml:namespace prefix = v ns = "urn:schemas-microsoft-com:vml" />
图1
XP MODE的虚拟机启动后，我们先在虚拟机中运行一下IE浏览器。如图2所示，这是一个正宗的IE6.0，我们在测试兼容性问题时经常会请它出马。因为IE8.0和很多网银都不兼容。有了XP MODE，我们就可以放心地使用Windows 7了。遇到支付宝这样的问题，请出XP MODE就万事大吉了。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539631411.jpg)
图2
解决了IE的问题后，我们来看看Windows Virtual PC的一些新特性。Windows Virtual PC可以支持USB设备了，这是个好消息，我的物理机上有个老型号的摄像头，由于只有XP的驱动程序，因此在Windows 7下已经罢工很久了。现在正好可以在XP MODE下体验一下，如图3所示，在Windows Virtual PC下点击虚拟机的USB菜单，我们看到XP MODE已经发现了一个PC Camera设备。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539640210.jpg)
图3
在XP MODE中发现了USB摄像头后，我们为摄像头安装驱动程序，如图4所示，我们很顺利地完成了驱动的安装。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539650882.jpg)
图4
在虚拟机中卡开摄像头，如图5所示，哈哈，XP MODE让这些老设备也有了重见天日的机会。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539666945.jpg)
图5
对XP MODE虚拟机有了一些认识之后，我们来看看XP MODE的配置，在Windows Virtual PC中点击虚拟机顶端“工具”栏中的“设置”，就可以看到如图6所示的配置界面。这个界面和Microsoft Virutal PC的配置界面非常类似，有VPC使用经验的用户很容易上手。我们从配置界面中看到，XP MODE虚拟机预分配了256M内存，基本可以保证XP的运行需求。显然，微软没有考虑到在XP虚拟机中运行3D游戏等复杂程序。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539677064.jpg)图6
如图7所示，在虚拟机的配置中我们看到网络适配器默认使用了NAT方式。这种方式可以让XP虚拟机中无需进行任何网络配置就可以通过物理机访问互联网，但要注意的是，如果通过NAT方式访问某些网站，例如登录招商银行，NAT会被认为是一种不被支持的危险的访问方式。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539687619.jpg)
图7
如图8所示，在虚拟机的配置中我们可以通过集成功能来调用物理主机上的诸多设备，例如剪贴板，打印机，磁盘驱动器以及智能卡。在虚拟机中直接访问物理主机上的磁盘驱动器是一个很好的设计，虚拟机访问物理主机上的磁盘就象访问本地磁盘那样方便。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539696443.jpg)
图8
如图9所示，XP MODE的配置中有一项自动发布，自动发布就是传说中的应用程序无缝集成功能。也就是说，在XP MODE虚拟机中安装的应用程序可以直接发布到Windows 7的物理主机上，在Windows 7的物理主机中可以直接执行XP MODE虚拟中的应用程序。下面我们举个例子为大家演示一下自动发布。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539704351.jpg)
图9
如图10所示，我们在XP MODE虚拟机中安装了Office2003，我们准备把Office2003中的Word发布到Windows 7中。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539715514.jpg)
图10
发布Word的方法非常简单，如图11所示，我们只要把Word的快捷方式复制到虚拟机中All Users用户配置文件的开始菜单中就可以了。有一定要注意的是，如果程序安装时询问这个程序是只有安装者才能使用还是所有用户都可以使用，一定要选择所有用户都可以使用。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539723785.jpg)
图11
把Word的快捷方式复制到All Users用户的开始菜单后，如图12所示，我们在Windows 7的Virtual Windows XP应用程序下就可以发现Word 2003了。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539732006.jpg)
图12
执行一下Word试试，如图13所示，Windows Virtual PC提示正在初始化虚拟环境，其实Windows Virtual PC正在后台准备启动XP MODE虚拟机。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539741515.jpg)图13
Word的执行结果如图14所示，我们感觉Word就象是直接运行在Windows 7中，程序运行得非常流畅，其实Word是在后台的XP 虚拟机上运行的。考虑到硬件成本逐步降低，分配一些资源给给XP MODE虚拟机，从而换取Windows 7拥有更好的应用程序兼容性，这种系统设计的策略还是可行的。
![](http://yuelei.blog.51cto.com/attachment/200907/200907141247539763342.jpg)
