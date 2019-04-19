# Ubuntu16.04 Jupyter安装（Ipython Notebook安装） - 数据之美的博客 - CSDN博客
2017年12月18日 16:55:06[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：534
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)
Jupyter Notebook是一个Web应用程序，允许您创建和共享包含实时代码，方程，可视化和说明文本的文档。
简单的介绍就是：Jupyter Notebook是Ipython的升级版，而Ipython可以说是一个加强版的交互式 Shell，也就是说，它比在terminal里运行python会更方便，界面更友好
环境：
Ubuntu16.04 64位
## 1. 安装pip
（1）更新和升级包
[java][view
 plain](http://blog.csdn.net/jenyzhang/article/details/73275232#)[copy](http://blog.csdn.net/jenyzhang/article/details/73275232#)
- sudo apt-get update  
- sudo apt-get upgrade  
（2）安装pip（或者pip3）

[java][view
 plain](http://blog.csdn.net/jenyzhang/article/details/73275232#)[copy](http://blog.csdn.net/jenyzhang/article/details/73275232#)
- sudo apt-get install python-pip  
- （如果是安装pip3的话，命令则是sudo apt-get install python3-pip）
## 2. 安装Jupyter
（1）升级pip
[java][view
 plain](http://blog.csdn.net/jenyzhang/article/details/73275232#)[copy](http://blog.csdn.net/jenyzhang/article/details/73275232#)
- sudo pip install --upgrade pip  
（2）安装Jupyter

[java][view
 plain](http://blog.csdn.net/jenyzhang/article/details/73275232#)[copy](http://blog.csdn.net/jenyzhang/article/details/73275232#)
- sudo pip install jupyter 
- （对应pip3的话，命令则是sudo pip3
 install jupyter） 
注意这里一定要有sudo。否则Jupyter安装会失败 
安装成功会显示下面的信息
![](https://img-blog.csdn.net/20170615102421408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3.运行Jupyter
[java][view
 plain](http://blog.csdn.net/jenyzhang/article/details/73275232#)[copy](http://blog.csdn.net/jenyzhang/article/details/73275232#)
- jupyter notebook  
会直接在浏览器打开jupyter notebook
![](https://img-blog.csdn.net/20170615102434752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170615102459862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来就可以使用啦！
## 4. 打开terminal
在右上角“new”这里，可以新建terminal，或者新建python交互窗口。
![](https://img-blog.csdn.net/20170615103513361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击“terminal”可以看到新打开了一个终端，在这里可以进行任何操作：
![](https://img-blog.csdn.net/20170615103447254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击“python”可以看到新打开了一个python2的ipython交互窗口：
![](https://img-blog.csdn.net/20170615103503176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamVueXpoYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
{注意}：
（1）在Windows下jupyter不能使用terminal!
（2）我尝试过在ubuntu12.04上安装jupyter，但是不成功。
【参考资料】
Jupyter官网：[https://jupyter.readthedocs.io/en/latest/install.html](https://jupyter.readthedocs.io/en/latest/install.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
