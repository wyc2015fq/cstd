# Linux/ubuntu目录解释 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月16日 13:17:33[boonya](https://me.csdn.net/boonya)阅读数：536









/bin:bin是binary(二进制)的缩写。这个目录是对UNIX系统习惯的沿袭，存放着使用者最经常使用的命令。例如：cp,ls,cat。

/boot:这里存放的是启动LINUX时使用的一些核心文件。

/dev:dev是device(设备)的缩写。这个目录下是所有LINUX的外部设备，其功能类似DOS下的.sys和Win下的.vxd。在LINUX中设备和文件是用同种方法访问的。例如:/dev/hda代表第一个物理IDE硬盘。

/etc:这个目录用来存放所有的系统管理所需要的配置文件和子目录。

/home:用户的主目录，比如说有个用户叫fcola，那他的主目录就是/home/fcola也可以用~fcola表示，说到这里，打个岔，你现应该明白在我们访问一些个人网页(如[http://www.ubuntuhome.com/~fcola)](http://www.ubuntuhome.com/~fcola%29)的时候，~fcola就是表示访问[www.fcola.cn](http://www.fcola.cn/)站点中的用户fcola的用户主目录。
 如果这个网站的操作系统是LINUX，那就是表示/home/fcola。


/lib:这个目录里存放着系统最基本的动态链接共享库，其作用类似于Windows里的.dll文件。几乎所有的应用程序都需要用到这些共享库。

/lost+found:这个目录平时是空的，当系统不正常关机后，这里就成了一些无家可归的文件的避难所。对了，有点类似于DOS下的.chk文件。

/mnt:这个目录是空的，系统提供这个目录是让用户临时挂载别的文件系统。

/proc:这个目录是一个虚拟的目录，它是系统内存的映射，我们可以通过直接访问这个目录来获取系统信息。也就是说，这个目录的内容不在硬盘

上而是在内存里啊！？

/root:系统管理员，也叫作超级权限者的用户主目录。当然系统的拥有者，总要有些特权啊！

/sbin:s就是Super User的意思，也就是说这里存放的是一些系统管理员使用的系统管理程序。

/tmp:这个目录不用说，一定是用来存放一些临时文件的地方了。

/usr:这是个最庞大的目录，我们要用到的很多应用程序和文件几乎都存放在这个目录下。具体来说：

/usr/X11R6 存放X-Windows的目录；

/usr/bin存放着许多应用程序；

/usr/sbin 给超级用户使用的一些管理程序就放在这；

/usr/doc这就是LINUX文档的大本营；

/usr/include LINUX下开发和编译应用程序需要的头文件，在这找；

/usr/lib 存放一些常用的动态链接共享库和静态档案库；

/usr/local 这是提供给一般用户的/usr目录，在这安装软件最适合；

/usr/man man是什么，对了是帮助。这里就是帮助文档目录啊！

/usr/src LINUX开放的源代码，就存在这个目录，爱好者们别放过哦！

/var:这个目录中存放着那些不断在扩充着的东西，为了保持/usr的相对稳定，那些经常被修改的目录可以放在这个目录下，实际上许多系统管理员都是这样干的。顺带说一下系统的日志文件就在/var/log目录中。




转自[Ubuntu之家](http://www.ubuntuhome.com/) {[www.ubuntuhome.com](http://www.ubuntuhome.com/)}




