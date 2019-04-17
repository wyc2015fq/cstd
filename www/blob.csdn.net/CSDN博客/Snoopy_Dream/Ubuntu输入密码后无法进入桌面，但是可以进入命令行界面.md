# Ubuntu输入密码后无法进入桌面，但是可以进入命令行界面 - Snoopy_Dream - CSDN博客





2018年03月12日 21:14:33[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：7906








安装中文版的ubuntu经常会出现无法进入桌面的状况，这里给出我的解决方案。

初步断定是Xwindows界面软件出问题了，所以重装即可！

Solve: 


**1.Ctrl+Alt+F1进入命令行界面，root账户登陆**


Ctrl+Alt+F1进入命令行界面后，首先出现  

ygx(你的用户名) login** ：**（在这里输入你的账户名）


![](https://img-blog.csdn.net/20180425153346251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


之后按照提示，下一步输入你的root的密码。

![](https://img-blog.csdn.net/20180425153520386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


注意在输入命令的时候会出现菱形◆◆◆◆◆◆这种的，是因为ubuntu在Ctrl+Alt+F1不支持中文，导致乱码。

最后就进入终端了，可以通过输入命令，来重装桌面。

![](https://img-blog.csdn.net/20180425153559760?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**2.卸载Xwindows软件 **

**sudo apt-get autoremove ubuntu-desktop**

**3.安装所有依赖软件**

sudo apt-get install ubuntu-desktop(安装到这一步，reboot尝试可以登入界面，不行就执行下面上那个命令)


Sudo apt-get install python-ntdb


Sudo apt-get install libntdb1:amd64


	    Sudo apt-get install libupstart1:amd64

**4.重启  reboot**

起初的界面又回来了，而且风格设置都没变，linux真是强大！









