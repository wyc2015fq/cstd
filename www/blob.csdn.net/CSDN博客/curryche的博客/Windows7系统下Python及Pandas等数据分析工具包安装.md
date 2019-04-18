# Windows7系统下Python及Pandas等数据分析工具包安装 - curryche的博客 - CSDN博客





2018年06月15日 10:08:45[curryche](https://me.csdn.net/whwan11)阅读数：473








## 1.系统及Python版本

本人的电脑安装的是64位windows7旗舰版，选择安装的是64位的Python3.5，之所以选择64位的Python是考虑到32位的Python能够支配的内存只有2G，当数据集过大时会爆内存，因此建议尽可能安装64位的Python，至于Python的版本则见仁见智，如果要用到TensorFlow的话那么只能安装3.5及以上。而且一台电脑上多个Python可以共存，这个无需纠结。

## 2.Python3.5下载及安装

[Python的下载地址](https://www.python.org/downloads/windows/)，拉到下面，选择3.5.2版本的Download Windows x86-64 executable installer。 
![](https://img-blog.csdn.net/20180608164537176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下载好之后，双击打开安装文件 
![这里写图片描述](https://img-blog.csdn.net/20180615093654609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择个性化安装 
![这里写图片描述](https://img-blog.csdn.net/20180615093812581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择下一步 
![这里写图片描述](https://img-blog.csdn.net/20180615093907449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择安装路径，可以勾选为这台计算机的所有用户安装（非必选项）并把python路径添加到环境变量中（不是必选项，python路径可手动添加到环境变量中）。 

最后点击安装。 
![这里写图片描述](https://img-blog.csdn.net/20180615094234992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装完成后，打开CMD 
![这里写图片描述](https://img-blog.csdn.net/20180615094351318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输入python，查看python版本信息。 
![这里写图片描述](https://img-blog.csdn.net/2018061509450298?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，python安装完成。
## 3.利用pip3安装Pandas等数据分析工具包

打开python安装路径 
![这里写图片描述](https://img-blog.csdn.net/20180615095241775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

进入Scripts 
![这里写图片描述](https://img-blog.csdn.net/20180615095343385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到pip3.exe，这就是我们用来安装python包的工具，在CMD窗口输入cd pip3.exe所在的路径 
![这里写图片描述](https://img-blog.csdn.net/20180615095541483?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装的命令是“pip3 install  工具包名字”，例如要安装pandas，则输入“pip3 install pandas”，pip3会自动下载并安装工具包。 

此外，“pip3 list”可以查看当前路径下的python已经安装的工具包。 
![这里写图片描述](https://img-blog.csdn.net/20180615100736746?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

完~~




