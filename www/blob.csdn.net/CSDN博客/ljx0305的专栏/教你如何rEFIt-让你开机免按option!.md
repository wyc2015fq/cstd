# 教你如何rEFIt-让你开机免按option! - ljx0305的专栏 - CSDN博客
2013年07月06日 09:34:08[ljx0305](https://me.csdn.net/ljx0305)阅读数：7276
                
苹果下refit菜单启动项使用方法
先安装refit你[下载](http://www.2cto.com/soft)的文件搞好后出弹出以下窗口，运行那个refit。
mpkg，跟着就是用refit创建启动菜单
在终端下输入cd ／，然后输入cd efi,然后输入cd refit，
然后输入sh enable-always.sh，跟着输入你bogon密码，大功告成.
![](http://apple.tgbus.com/UploadFiles/200806/20080611142824314.jpg)
　　我想很多人都在Intel Mac上装了windows吧，那么是不是那些经常换系统用的朋友每次都在按OPTION呢？
　　有没有办法可以像在WIN那样让他在几秒内自动出来菜单让们选呢 ？
　　方法是有的，那就是借助rEFIt,下面我们就简单介绍一下使用的方法
　　先声明软件存在风险，出现问题本站不负任何责任!!
　　下载完后，打开映像后，运行里面的mpkg文件。
![](http://apple.tgbus.com/UploadFiles/200806/20080611143237451.jpg)
　　之后按照提示，继续，下一步。。完成。
![](http://apple.tgbus.com/UploadFiles/200806/20080611143527351.jpg)
　　完成安裝之後，重启，会出现如下画面，现在你就就可以不用OPTION也选择系统了。
![](http://apple.tgbus.com/UploadFiles/200806/20080611143903400.jpg)
 　　另外默认的等待时间为20秒，如果想更改这个时间。打开\efi\refit里面refit.conf文件，选择用文本编辑打开。将timeout 20中的20改为其他时间，如30为30秒。
![](http://apple.tgbus.com/UploadFiles/200806/20080611151312343.jpg)
　　2.如果你不想用了，想删除rEFIt,先打开系统偏好设置的“启动磁盘”，在其中设置为你要默认启动的系统
![](http://apple.tgbus.com/UploadFiles/200806/20080611151406264.jpg)
　　然后将主磁盘的EFI文件夹改名或删除
![](http://apple.tgbus.com/UploadFiles/200806/20080611151456118.jpg)
　　如果你安装或删除rEFIt出现问题，不能自动进入操作系统，可以用插入Mac OS X install disc的CD1，重新开机之后按住c用光碟开机，进入安装画面之后，选择上方工具栏中的”磁盘工具”>>”启动磁盘”，然后选择你要默认启动的系统，然后重启一下就可以了。
