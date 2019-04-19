# 【Win7+VirtualBox搭建Mac环境】+ 摒弃error "starting darwin x86" - 三少GG - CSDN博客
2013年03月13日 13:32:58[三少GG](https://me.csdn.net/scut1135)阅读数：4163
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)
virtualbox  **error: starting darwin x86**
**==============================================================================================================**
### [How to install OS X Mountain Lion in Virtualbox with iAtkos](http://www.macbreaker.com/2013/01/iatkos-ml2-mountain-lion-virtualbox.html)
可实施其中step 1-5 !
step 6添加声音，可选！(未安装)
step 7 设置分辨率。 1. 右键cmd用管理员权限打开 2.  VBoxManage setextradata "MacOS-VM" "CustomVideoMode1"**"1280x768x32"  即可（成功）。但是分辨率有要求，要尝试几次，比如1366x768x32就不行。**
![](https://img-my.csdn.net/uploads/201303/17/1363516668_5502.jpg)
 ===============================================================================================================
~~但在使用过程中我们会发现这样一个问题，有些VirtualBox中的MAC总是无法全屏显示(通过VirtualBox的工具包无效的情况)，看起来不太舒服，而且没办法充分利用显示器的最大化空间，因此本文即兴移动给大家介绍设置VirtualBox中的MAC全屏显示的方法。~~
~~1.~~
~~首先是给VirtualBox添加一个新的自定义分辨率。通过cmd命令进入VirtualBox的安装目录，有个命令行工具VBoxManger.exe，利用这个工具给mac虚拟机增加一个自定义分辨率，执行语句格式为VBoxManage setextradata mac CustomVideoMode1 1280x800x32，命令行里mac是我的mac虚拟机的名字，最后是自定义的分辨率，注意最后是mode+1不是model，然后那个1也可以随便改成其他数字的。~~
~~2.~~
~~当在VirtualBox中启动MAC的时候，进入启动界面后，先按右方向键切换到第二个启动图标，则MAC启动项，此时不要马上按回车进入系统，直接输入"Graphics Mode"="1366*768*32"，注意输入包括双引号，输入完毕后直接回车~~
~~通过以上的设置之后，即可成功将MAC的显示最大化，适配了显示器的分辨率，以上参数的中的1366×768修改为实际的屏幕显示器即可。这个方法的设置虽然简单，但是**每次重启都需要重新进行设置，**在这里我们建议如果你的虚拟机工具包无法设置最大化的情况下，使用本文章的方法进行设置的话，每次都不必关闭系统，而是**通过VirtualBox保存操作系统的状态即可，这样下次启动也能更快速的进入系统。**~~
~~之后重启，就可以用你设定的分辨率了。不过说一下，并不是所有分辨率都有效，比如我的本本是1366x768的，**总是无法成功，改成1360x768就好了**。不晓得你是什么分辨率，比较大众的1280x720，1920x1080，1024x768这些应该可以。你试试看吧，祝你好运！~~
utility -->terminal
**切换至Insert mode编辑文件在Command mode下按‘i’、‘a’或‘o’三键就可进入Insert mode。**这时候您就可以开始输入文字了。
i: 插入，从目前光标所在之处插入所输入的文字。
a: 增加，目前光标所在的下一个字开始输入文字。
o: 插入新的一行，从行首开始输入文字。
**Insert的切换→Command mode，按Esc键**
您目前处于Insert mode，您就只能一直打字。假如您发现打错字了，想用光标键往回移动，将该字删除，
就要按ESC键转换回Command mode，再删除文字。
离开vi及存文件
在Command mode下，可按冒号“：”键入入Last line mode，例如：
:w filename (输入“w filename”，将文章存入指定的文件名filename)
:wq (输入“wq”，因为进入之时已经指定文件名testfile，所以会写入testfile并离开vi)
:q! (输入“q!”，强制离开并放弃编辑的文件)
[http://iatkos.me/iatkos-releases/](http://iatkos.me/iatkos-releases/) 下载： [iATKOS
 ML2(Mountain Lion
 10.8.2)December 2012](http://iatkos.me/forum/viewtopic.php?f=77&t=23105)
**http://www.tonymacx86.com/         注册下载:[Multibeast 4.6.1](http://www.tonymacx86.com/downloads.php?do=cat&id=6)     archive: http://www.tonymacx86.com/downloads.php?do=cat&id=6**
###### [Rate this File]()
Uploaded by [tonymacx86](http://www.tonymacx86.com/members/tonymacx86/) -
 July 24th, 2012
||Author|tonymacx86.com||----|----||File Size|28.68 MB||Author|tonymacx86.com|File Size|28.68 MB|[](http://www.tonymacx86.com/downloads.php?do=file&id=34&act=down&actionhash=1363153988-4a90f9b9276ac487e5b942a82c56c6086d1ef4ed)Download|
|----|----|----|----|----|----|
|Author|tonymacx86.com| | | | |
|File Size|28.68 MB| | | | |
[](http://www.tonymacx86.com/downloads.php?do=file&id=34&act=down&actionhash=1363153988-4a90f9b9276ac487e5b942a82c56c6086d1ef4ed)
Download
> The Ultimate Post-Installation Utility- for 10.7.x
![](http://www.tonymacx86.com/images/downloads/multibeast.png)

如何更改鼠标滑轮的滚动方向？跟windows相反。
将鼠标设置里面的最上面那个选项取消掉。
**[www.guomii.com/posts/4149](www.guomii.com/posts/4149)**
**==================================================================================================**
**坑爹的国内翻译博客啊，再一次失去信心： **
**所提方法均无法解决 **starting darwin x86 屏幕卡住一直黑屏的问题。****
**~~eg: 1.  virtualbox设置成 Mac Os 64       2. 设置boot选项 busratio=20       3.~~**
**[全民吃苹果，首发VirtualBOX安装Mac OS X 10.7正式版_可完美升级，无bug](http://ideapad.zol.com.cn/56/160_557572.html)**
# [【超多图，超详解】在virtualbox上安装Mac OS X Lion 之 安装过程](http://www.crifan.com/install_mac_os_x_lion_on_virtualbox_install_process/)
# [【继续折腾】在K470+SSD+Win7下，使用VirtualBox安装Mac Lion 10.7](http://www.crifan.com/continue_install_mac_lion_10_7_on_virtualbox_under_k470_ssd_win7/)
