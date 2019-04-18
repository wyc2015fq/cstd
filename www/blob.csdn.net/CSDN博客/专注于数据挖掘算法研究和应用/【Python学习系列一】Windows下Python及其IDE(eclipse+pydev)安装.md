# 【Python学习系列一】Windows下Python及其IDE(eclipse+pydev)安装 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月06日 16:35:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3073
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









                
为了爬虫和数据分析工作，python无可避免要学习。



1、Python语言的特点：

   1)Python 是一种解释型语言： 这意味着开发过程中没有了编译这个环节。类似于PHP和Perl语言。

   2)Python 是交互式语言： 这意味着，您可以在一个Python提示符，直接互动执行写你的程序。

   3)Python 是面向对象语言: 这意味着Python支持面向对象的风格或代码封装在对象的编程技术。

   4)Python 是初学者的语言：Python 对初级程序员而言，是一种伟大的语言，它支持广泛的应用程序开发，从简单的文字处理到www浏览器再到游戏。



2、Python下载和安装，选择2.7版本稳定且支持各种包

   https://www.python.org/downloads/release/python-2712/

   下载python-2.7.12.amd64.msi按步骤安装，记得勾选Add Python 3.5 to PATH选项安装，避免设置path环境变量；

    默认安装在C:\Python27目录下。

    windows下打开cmd，直接输入python命令即可。如下图：

![](https://img-blog.csdn.net/20170606163402324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

PS：Windows中path环境变量设置，在环境变量中添加Python目录：

       在命令提示框中(cmd) : 输入path=%path%;C:\Python27按下"Enter"。





3、集成开发环境安装

   习惯用eclipe作为开发环境，看看有没python for eclipse插件安装，选择Eclipse+PyDev来搭建。

   1)PyDev插件安装

   JDK1.7+Eclipse Mars（Version: Mars.1 Release (4.5.1)）版本安装就不说了，注意PyDev和eclipse及jdk有版本匹配的。

   PyDev官网 http://www.pydev.org/

   历史版本下载网址：http://www.pydev.org/update_sites/

   选择4.5.5版本，下载网址：https://dl.bintray.com/fabioz/pydev/4.5.5/

   eclipse在线安装，打开Eclipse，选择【Help】-【Install New Software】，如下图设置并按步骤安装即可。


![](https://img-blog.csdn.net/20170606163502700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




也可离线安装，下载离线包解压，得到plugins和features文件夹，放到Eclipse的dropins目录下。


   2)配置Python的解释器

    打开Eclipse，选择【Window】-【Preferences】，找到【PyDev】，选择其中的【Interpreter – Python】。

    点击【New】，添加一个系统里已有的Python解释器的位置。

    确定后会经过短暂的处理，得到它的Libraries、Buildins等。



4、Python开发

   1)点击【File】-【New】-【Other】，找到【PyDev】，选择【PyDev Project】，点击Next，新建项目；

   2)右键项目的src目录，选择【New】-【PyDev Package】，创建一个Python包；

     再右键该package，【New】-【PyDev Module】，创建py文件；

   3)打开py文件，编写代码，如print 'hello python!'

     右键项目，选择【Run As】-【Python Run】，或Ctrl+F11运行项目。



5、后续开发重点是python用于数据分析和挖掘，重点是三个库的学习

   1)numpy：http://www.numpy.org/

   2)scipy:https://www.scipy.org/

   3)pandas:https://pandas.pydata.org/




