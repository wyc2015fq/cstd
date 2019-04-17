# python︱Anaconda安装、简介（安装报错问题解决、Jupyter Notebook） - 素质云笔记/Recorder... - CSDN博客





2016年10月26日 13:55:38[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：22991










**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


安装完anaconda，就相当于安装了Python、IPython、集成开发环境Spyder、一些包等等。是一个IDE、安装包的大杂烩，很好很强大，官方下载链接：https://www.continuum.io/downloads



![](https://img-blog.csdn.net/20161026134023057)


下载后，直接安装即可，炒鸡方便。




————————————————————————————————————————





问题一：里面的python是下载Anaconda后就直接出现了的吗？




是的，同时还帮你下载了python 以及ipython,同时也可以选择下载得版本。




————————————————————————————————————————





问题二：Anaconda下载了之后，安装的时候出现报错：





**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52205291#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[?](http://blog.csdn.net/sinat_26917383/article/details/52205291#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- already exists，please choose a different location


总是只能安装到默认的下载路径，就是自己不能设置、选择Anaconda的安装路径。

**解决办法：文件名的问题**

可以直接在安装路径中填写，比如你想装在F盘，但是需要新建一个文件夹，假设文件夹名为Anaconda，你就在浏览中选择F盘，然后在路径后面补充Anaconda就可以，比如：F:\Anaconda





————————————————————————————————————————




## 一、Anaconda配套软件的简介




Anaconda加载好之后，有一堆新的东西，这些东西是啥？（主要参考博客：[Windows下Anaconda的安装和简单使用](http://www.th7.cn/Program/Python/201507/515289.shtml)）






# 1、Python（shell），跟常规以前下载的python

2、IPython(shell)，和我们pip install ipython安装的ipython用法一样
3、Ipython QTConsole（新版：JupyterQTConsole），类似于Python GUI，将ipython装饰得更好看一些。


4、 IPython Notebook（新版：Jupyter Notebook），可编辑、可视化程度高的文档类编译方式。

5、Spyder（IDE），堪比Rstudio的IDE，我也找过很多python的IDE，据说这款很像matlab，而且也有点Rstudio的感觉，我觉得很不错。

6、Anaconda prompt。如果要pip加载包，需要用这个命令窗口，跟CMD一个道理。只不过，可以将包安装在Anaconda认识的路径下。

7、Anaconda Navigator。类似安装包以及安装环境的一个集合。然后点击Environment，root就是我们安装的环境，py35是我后来安装的环境。




![](https://img-blog.csdn.net/20161026132448694)





————————————————————————————————————————





## 二、其他的IDE使用anaconda




由于安装完anaconda就自带了Spyder集成开发环境了，所以不需要任何配置可以直接使用，但是其他你自己安装的IDE要想使用anaconda需要配置。 


配置的方法我们可以去[官网](http://docs.continuum.io/anaconda/ide_integration)了解。下面我们以Pycharm为例。 


其实如果anaconda在Pycharm之前安装，那么遇到解释器选择的时候会有一个选项，我们直接选择就OK了。如果在Pycharm之后安装，我们只需要手动设置一下Pycharm所用的解释器就行了。 


![](https://img-blog.csdn.net/20161026121906980)





——————————————————————————————————————————————————————





## 三、自带的安装包以及安装方式




其实安装完anaconda后，基本上就把我们常用的部分包安装好了，但是毕竟只是安装了一小部分（我们可以去官网查看安装了哪些包），还有一部分需要我们自己来安装。 
查看已经安装的包我们可以在命令提示符中输入pip list或者用anaconda自带的包管理器conda（conda list）。就可以看到包的列表。





我们可以在命令提示符中输入**pip install 包名，或者conda install 包名**。




conda和pip的用法基本上一致（更新？卸载？还不确定）。但是我们使用conda安装的时候不仅会安装当前你要安装的包还会提示更新其他已经安装过的包，所以我基本上都是直接用pip安装。





pip或conda不能安装的话，我们就下载文件安装，比如exe文件（双击安装）或者whl文件（pip安装）等等。


从而，里面一些安装包是没有的：scikit-learn、nltk ， jieba 、 gensim 

敲的代码是：





**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52205291#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[?](http://blog.csdn.net/sinat_26917383/article/details/52205291#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- conda install scikit-learn  

书写格式：



**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52205291#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52205291#)[?](http://blog.csdn.net/sinat_26917383/article/details/52205291#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- conda install name  
- name是需要安装packages的名字，比如，我安装numpy包，输入上面的命令就是：  
- 
- conda install numpy  
- 单词之间空一格，然后回车，输入y就可以了  


——————————————————————————————————————————————————————




# 四、Jupyter Notebook——最好的编译笔记本







一般堆代码也都是在Ipython Notebook里面完成的，很少去Spyder里面编辑，毕竟能少打开一个软件窗口就要节省一个窗口，而且整合在浏览器中的Ipython Notebook对代码每一步的测试很好用，命令行和批处理完美的融合在了一起

而且学会了Markdown语言后，整个就是一个笔记本，无论是图片，视频，数学公式，还是项目列表，表格，各种格式的文本都可以在一个ipnb文件里完成。是**科学工作者和程序员的好笔记本**，只有更爱它。（本节来源[知乎链接](https://www.zhihu.com/question/27615938/answer/63803282)）





**调出方式**
点击Jupyter Notebook后，会跳出以下网页，
![](https://img-blog.csdn.net/20161026134531655)


点击python scripts——untitled.ipynb就可以调用了。

![](https://img-blog.csdn.net/20161026135108511)


可以插入图片


可以嵌入网络链接


可以插入网页


可以直接绘图


对 pandas 的良好支持


可以编辑数学公式





之前不小心误删了信息，怎么恢复？



```python
for line in locals()['In']:
    print(line)
```






—————————————————————————————————————




# 应用一：如何在Linux系统中加载Anaconda？




      我在虚拟机里面先下载了Anaconda的安装包，由于放在了Ubuntu 16.04的桌面，需要找到这个桌面位置，然后执行以下的命令：


`bash Anaconda2-4.2.0-Linux-x86_64.sh`      我的虚拟机里面是：



`cd /home/设置名称/Desktop`






