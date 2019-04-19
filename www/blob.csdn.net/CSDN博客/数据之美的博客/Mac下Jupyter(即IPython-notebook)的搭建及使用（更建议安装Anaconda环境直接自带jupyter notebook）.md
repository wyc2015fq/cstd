# Mac下Jupyter(即IPython-notebook)的搭建及使用（更建议安装Anaconda环境直接自带jupyter notebook） - 数据之美的博客 - CSDN博客
2018年01月23日 13:27:19[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2489
ipython是一个交互式的shell，除了代码补全，代码高亮外，还有很多magic命令。但是，最近在学习scipy时，发现很多学习文档使用的都是ipython-notebook，它除了拥有ipython的特点外，还支持多行输入统一运行（区别于ipython等一行输入一行运行），能够直接在当前界面显示绘制的图形，能够将编写的'.ipynb'文件复制到其他拥有notebook的环境下运行现实。当然，它是在网页上进行编辑和显示运行效果的，经过设置也可以远程进行访问编辑和运行。
在我搜索安装文档时，发现别人的博客中记录的都不太一样，大多数都是手动的安装所必需的模块。当我查看ipython官网时，我发现了官网中提供了更简单的安装方式。ipython-notebook已经整合（或者改名）到“Jupyter”中了。在http://jupyter.readthedocs.org/en/latest/install.html这个网站上，有着极其简便的python-notebook的安装方式。
因为在之前已经安装了pip和ipython，可以使用pip命令进行安装：
1.Jupyter搭建
**[plain]**[view
 plain](http://blog.csdn.net/cruise_h/article/details/60337437#)[copy](http://blog.csdn.net/cruise_h/article/details/60337437#)
- pip install --user jupyter  
如果是在python3中，则用如下命令：
**[plain]**[view
 plain](http://blog.csdn.net/cruise_h/article/details/60337437#)[copy](http://blog.csdn.net/cruise_h/article/details/60337437#)
- pip3 install --user jupyter  
如下图所示：
![](https://img-blog.csdn.net/20170305001646362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170305001748448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. Jupyter使用：
安装完成后，在命令行中输入：
**[plain]**[view
 plain](http://blog.csdn.net/cruise_h/article/details/60337437#)[copy](http://blog.csdn.net/cruise_h/article/details/60337437#)
- python -m IPython notebook  
![](https://img-blog.csdn.net/20170305001908121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时即启动了ipython-notebook，此时浏览器中会弹出notebook窗口（http://localhost:8889/tree）。如下图所示：
![](https://img-blog.csdn.net/20170305002417762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Files下面列出了当前用户目录下所有的文件，结合右上角的upload和new你可以方便地进行文件操作。你先选择一个目录，在该目录下，new一个python2，这样当前目录即会出现一个“.ipynb”文件，同时，浏览器会弹新的标签页让你编辑该“.ipynb”文件。如下图：
![](https://img-blog.csdn.net/20170305002509763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在该界面下，每输入一行代码Enter后不会运行当前代码，而是换行让你输入下一行代码，当你键入Shift + Enter后，将运行刚刚你输入的那几行代码。其他不懂的，可以点击界面Help栏中的User
  Interface Tour和Keyboard shortcut，它会告诉你界面中每个按钮的作用以及当前界面的快捷键作用。
同时，还可以new一个Terminal，如下图所示。
![](https://img-blog.csdn.net/20170305002542436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170305002621484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3J1aXNlX2g=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果想卸载刚才安装的jupyter notebook，可以命令行卸载：pip uninstall IPython notebook 即可
