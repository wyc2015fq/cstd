# 找回消失的ubuntu启动选项 - wishchinYang的专栏 - CSDN博客
2015年04月12日 19:32:53[wishchin](https://me.csdn.net/wishchin)阅读数：1062
启动菜单消失不是第一次了，这一次还是记录下来吧
原文链接：[http://blog.chinaunix.net/uid-26527046-id-3748986.html](http://blog.chinaunix.net/uid-26527046-id-3748986.html)
在安装windows后安装的ubuntu双系统，在启动时候都会固定先显示grub选择菜单，可以选择不同版本的ubuntu的kernel来启动
但是对于单纯的ubuntu系统，在启动后却不会显示相应的菜单来让选择。、
参照网络上搜索的资料，可以有以下方式来显示grub菜单（同理相信看完也了解如何隐藏了撒？？？）：
1.  启动时候一直按住shift键
以前的Ubuntu版本，启动grub的时候，有一个grub loading，press ESC to get the menu的提示，所以按ESC就可以看到grub界面
现在的Ubuntu没有了，现在的做法是，启动的时候按住shift键，就可以看到grub界面了。
（接下来就修改启动参数，加入single，然后按ctrl+x启动，就能看到一个ncurse的界面，选择drop to root prompt，就可以得到一个root的terminal然后就可以重新设置root口令了）
2.  修改grub文件来完成（红色加大选项**！！！**）
点击(此处)折叠或打开
- 1.sudo gedit /etc/default/grub
- 修改如下：
- GRUB_HIDDEN_TIMEOUT=0
- **#GRUB_HIDDEN_TIMEOUT_QUIET=true**
- GRUB_TIMEOUT=0
- 
- 2.sudo update-grub
下面展开说明：
首先修改 /etc/default/grub.cfg文件，参考以下config文件注释掉GRUB_HIDDEN_TIMEOUT=0
点击(此处)折叠或打开
- # If you changethis file, run'update-grub' afterwards to update
- # /boot/grub/grub.cfg.
- # For full documentation 
of the options in
this file, see:
- # info -f grub 
-n 'Simple configuration'
- 
- GRUB_DEFAULT=0
- ***#GRUB_HIDDEN_TIMEOUT******=******0***
- GRUB_HIDDEN_TIMEOUT_QUIET=true
- GRUB_TIMEOUT=10 
- GRUB_DISTRIBUTOR=`lsb_release-i
-s 2> /dev/null||echo Debian`
- GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
- GRUB_CMDLINE_LINUX=""
- 
- # Uncomment to enable BadRAM filtering, modify to suit your needs
- # This works with Linux 
(no patch required)
and with any kernel that obtains
- # the memory map information from GRUB (GNU Mach, kernelof FreeBSD
...)
- #GRUB_BADRAM="0x01234567,0xfefefefe,0x89abcdef,0xefefefef"
- 
- # Uncomment to disable graphical terminal (grub-pc only)
- #GRUB_TERMINAL=console
- 
- # The resolution used on graphical terminal
- # note that you can use only modes which your graphic card supports via VBE
- # you can see them in real GRUB with the command `vbeinfo'
- #GRUB_GFXMODE=640x480
- 
- # Uncomment if you don't want GRUB to pass "root=UUID=xxx" parameter to Linux
- #GRUB_DISABLE_LINUX_UUID=true
- 
- # Uncomment to disable generation of recovery mode menu entries
- #GRUB_DISABLE_RECOVERY="true"
- 
- # Uncomment to get a beep at grub start
- #GRUB_INIT_TUNE="480 440 1"
然后执行"sudo update-grub"。这样以后在grub.cfg中会多出以下代码
点击(此处)折叠或打开
- ### BEGIN /etc/grub.d/30_os-prober ###
- if[
"x${timeout}"!="x-1"]; then
- if keystatus; then
- if keystatus 
--shift; then
-       set timeout=-1
- else
-       set timeout=0
-     fi 
- else
- ifsleep--interruptible 3; then
-       set timeout=0
-     fi 
-   fi 
- fi
- ### END /etc/grub.d/30_os-prober ###
最后简单介绍下其中的几个参数
点击(此处)折叠或打开
- ***1******.******GRUB_HIDDEN_TIMEOUT******=******0***
-                 此配置将影响菜单显示。若设置此选项,将在此时间内隐藏菜单而显示引导画面。
-                 菜单将会被隐藏,除非在此行开头加上一个 # 符号。(# GRUB_HIDDEN_TIMEOUT=0)。
-                 GRUB 2 第一次执行时将会寻找其他操作系统。若没有其他操作系统被检测到,菜单将会配置为隐藏。若辨认出其他操作系统,菜单将会显示。
-                 若是大于 0 的整数,系统将会依此配置的秒数暂停,但不会显示菜单。
-                 0 则菜单不会显示,也不会有延迟。
-                 使用者可以在启动时按住 SHIFT 键不放以强制显示菜单。
-                 启动过程中,系统将会检查 SHIFT 键状态。若无法辨识按键状态,会有一个短时间的延迟让使用者可通过按下ESC 键来显示菜单。
- 
- ***2******.******GRUB_HIDDEN_TIMEOUT_QUIET******=******true***
-                  true 不显示倒计时。屏幕将会是空白的。
-                  false 在 GRUB_HIDDEN_TIMEOUT 中配置的时间,空白屏幕上会有一个倒数计时器。
- 
- ***3******.******GRUB_TIMEOUT******=******10***
-                   此命令将顺从 GRUB_HIDDEN_TIMEOUT 配置,除非 GRUB_HIDDEN_TIMEOUT 被注释掉(#)。若 GRUB_HIDDEN_TIMEOUT 启用,则当菜单显示时,GRUB_TIMEOUT
 将会只执行一次。
-                   配置此值为 -1 将会导致菜单一直显示,直到用户选择。
-                   GRUB 2 菜单默认为隐藏,除非其他操作系统被系统检测到。若没有其他操作系统,此行将会被注释掉,除非使用者修改它。为了在每次启动时显示菜单,去掉此行的注释并使用 1 或更大的值。
------------------------------------------------------------------------
Ref:
http://www.cnblogs.com/super119/archive/2011/03/11/1981596.html
http://forum.ubuntu.org.cn/viewtopic.php?f=48&t=375600
http://wiki.ubuntu-tw.org/?title=GRUB_2_%E4%B8%AD%E6%96%87%E6%8C%87%E5%8D%97
