# Jython的安装教程 - zhusongziye的博客 - CSDN博客





2018年07月11日 12:31:20[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1289








一、首先安装JDK，安装方法自行百度，可参考：

https://blog.csdn.net/wph199505110014/article/details/79254506

二、下载Jython

官网下载地址：http://www.jython.org/downloads.html

下载完成后找到安装包

![](https://img-blog.csdn.net/20180711121058544?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三、Jython安装

官网有安装步骤，网址：https://wiki.python.org/jython/InstallationInstructions




### [Jython 2.7.1](https://wiki.python.org/jython/InstallationInstructions#id1)


#### [Basic Install](https://wiki.python.org/jython/InstallationInstructions#id2)

Jython 2.7.1 is distributed via an executable jar file installer.  After[downloading](https://wiki.python.org/jython/DownloadInstructions) it, either double click thejython_installer-2.7.1.jar or run java with the -jar option
java -jar jython_installer-2.7.1.jar
This will start the regular GUI installer on most systems, or aconsole installer on headless systems.  To force the installer towork in headless mode invoke the installer as:
java -jar jython_installer-2.7.1.jar --console
The installer will then walk through a similar set of steps ingraphical or console mode: showing the license, selecting an installdirectory and JVM and actually copying Jython to the filesystem.After this completes, Jython is installed in the directory youselected.  Executing a script in the install directory, jython on Unix-like systems or jython.exe on Windows, will start up the Jythonconsole, which can be used to dynamically explore Jython and the Javaruntime, or to run Jython scripts.



#### [Standalone mode](https://wiki.python.org/jython/InstallationInstructions#id3)

The standalone option does no caching and so avoids the startup overhead (most likely at the cost of some speed in calling Java classes, but I have not profiled it)

You can try it out by running the installer:
$ java -jar jython_installer-2.7.1.jar
then when you come to the "Installation type" page, select "Standalone".

The installation will generate a jython.jar with the Python standard library (/Lib) files included, which can be run as:
$ java -jar jython.jar
Of course you can run scripts just by calling them as you might expect:
$ java -jar jython.jar script.py
Or, add this file to the classpath of your application.



#### [Installation options](https://wiki.python.org/jython/InstallationInstructions#id4)

You can get a list of installer options (to install Jython unattended, for example) by running:
$ java -jar jython_installer-2.7.1.jar --help


上面是官方的安装说明，下面我们采用默认安装，上图：

![](https://img-blog.csdn.net/20180711121555976?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711121638289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711121706580?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711121933918?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018071112204117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711122236757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711122324513?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711122403589?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180711122614584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


![](https://img-blog.csdn.net/20180711122822333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

到此，jython安装完成！

![](https://img-blog.csdn.net/20180711123007174?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




