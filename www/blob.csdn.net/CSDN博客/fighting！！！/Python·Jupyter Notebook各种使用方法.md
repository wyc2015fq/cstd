# Python·Jupyter Notebook各种使用方法 - fighting！！！ - CSDN博客
2019年01月12日 17:01:22[dujiahei](https://me.csdn.net/dujiahei)阅读数：45
转自：[https://www.jianshu.com/p/21ba32a057c4](https://www.jianshu.com/p/21ba32a057c4)
[一、 Jupyter NoteBook的安装]()
[1.1 新版本Anaconda自带Jupyter]()
目前，最新版本的Anaconda是自带Jupyter NoteBook的，不需要再单独安装 
![](https://upload-images.jianshu.io/upload_images/8344711-1cdde4b07dbc6d13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/242/format/webp)
[1.2 老版本Anacodna需自己安装Jupyter]()
[Jupyter Notebook安装的官方网站](https://link.jianshu.com?t=http%3A%2F%2Fjupyter.readthedocs.io%2Fen%2Flatest%2Finstall.html)
安装Jupyter Notebook的**先决条件**：已经安装了python（python 2.7 或者是python3.3）
具体的安装方法：
官方建议利用Anaconda安装Jupyter
安装完成Anaconda后，如果该Anaconda并不自带Jupyter Noterbook，那么，打开cmd，输入：conda install jupyter
**这样安装完的jupyter不具有New a terminal的功能**
经过各种查询才知道，原来是因为windows不具有terminal需要的TTY，所以，windows下的jupyter是不支持Terminal模式的，而且短期内也没有增加这种支持的计划
![](https://upload-images.jianshu.io/upload_images/8344711-540909da83117d71?imageMogr2/auto-orient/strip%7CimageView2/2/w/278/format/webp)
[二、 更改Jupyter notebook的工作空间]()
[2.1 方式一]()
“”不是每次都好用“”
在其配置文件ipython_notebook_config.py中，有如下一句
# The directorytousefornotebooksandkernels.
# c.NotebookApp.notebook_dir = u''
该句就是用来指定其工作空间的，例如，默认的工作空间是：用户名文件夹，例如，现在想要将工作空间变为D:\Jupyter，那么，需要做如下更改(要记得删掉注释#)
# The directory to use for notebooks and kernels.
c.NotebookApp.notebook_dir =u'D:\Jupyter'
注意：路径最后一级后面不要加符号“\”
**如何找到该配置文件？**
在cmd中输入jupyter notebook --generate-config
如果该配置文件已经存在，那么，会出现如下信息,从中可以见到配置文件存在的位置，注意，此时，输入N，不要overwrite 
![](https://upload-images.jianshu.io/upload_images/8344711-7208538fff38e34a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/742/format/webp)
如果该配置文件不存在，那么，将会初始化产生一个配置文件
在cmd中输入：ipython profile create 
可以找到关于jupyter的配置文件的位置
[2.2 方式二绝招（绝招）]()
进入工作目录文件夹
键盘Shift+鼠标右键->在此处打开命令窗口-> 在弹出的命令窗口中输入：Jupyter Notebook 
![](https://upload-images.jianshu.io/upload_images/8344711-b7148be65714e97a?imageMogr2/auto-orient/strip%7CimageView2/2/w/689/format/webp)
![](https://upload-images.jianshu.io/upload_images/8344711-9e20781254a78e34?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Jupyter被打开，定位到当前目录！ 
![](https://upload-images.jianshu.io/upload_images/8344711-f0b5579ee062ec4d?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
[三、Jupyter的各种快捷键]()
执行当前cell，并自动跳到下一个cell：Shift Enter
执行当前cell，执行后不自动调转到下一个cell：Ctrl-Enter
是当前的cell进入编辑模式：Enter
退出当前cell的编辑模式：Esc
删除当前的cell：双D
为当前的cell加入line number：单L
将当前的cell转化为具有一级标题的maskdown：单1
将当前的cell转化为具有二级标题的maskdown：单2
将当前的cell转化为具有三级标题的maskdown：单3
为一行或者多行添加/取消注释：Crtl /
撤销对某个cell的删除：z
浏览器的各个Tab之间切换：Crtl PgUp和Crtl PgDn
快速跳转到首个cell：Crtl Home
快速跳转到最后一个cell：Crtl End
[四、Jupyter Notebook如何导入代码]()
即导入代码到jupyter notebook的cell中
[4.1 将本地的.py文件load到jupyter的一个cell中]()
**问题背景**：有一个test.py文件，需要将其载入到jupyter的一个cell中 
test.py内容如下：
importcaffe
SolverName ="/root/workspace"
sovler = caffe.AdamSolver(SolverName)
**方法步骤**： 
（1）在需要导入该段代码的cell中输入
%loadtest.py #test.py是当前路径下的一个python文件
1
![](https://upload-images.jianshu.io/upload_images/8344711-9dc3c80762a21ee6?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
（2）运行该cell 
利用快捷键“Shift+Enter”，可以看到如下结果： 
![](https://upload-images.jianshu.io/upload_images/8344711-30781c7c39795525?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
（3）可以看到，运行后，%load test.py被自动加入了注释符号#，test.py中的所有代码都被load到了当前的cell中
[4.2 从网络load代码到jupyter]()
在cell中输入%load http://.....，然后运行该cell，就会将load后面所对应地址的代码load到当前的cell中；
下面给出一个例子，导入[matplotlib](https://link.jianshu.com?t=http%3A%2F%2Fmatplotlib.org%2F)中的一个小例子[color example code](https://link.jianshu.com?t=http%3A%2F%2Fmatplotlib.org%2Fexamples%2Fcolor%2Fcolor_cycle_demo.html)
首先，在想要导入该段代码的cell中输入
%loadtest.py #test.py是当前路径下的一个python文件
1
然后，Shift+Enter运行，可以看到如下结果： 
![](https://upload-images.jianshu.io/upload_images/8344711-1f39ab3c65fa8dcb?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
可以看到，运行后，%load test.py被自动加入了注释符号#，test.py中的所有代码都被load到了当前的cell中
[五、Jupyter运行python文件]()
利用jupyter的cell是可以运行python文件的，即在cell中运行如下代码：
%runfile.py
1
file.py为要运行的python程序，结果会显示在该cell中 
![](https://upload-images.jianshu.io/upload_images/8344711-4fa9d2efd7062efe?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/8344711-ad55b1c0249a420c?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
[六、Jupyter一些其他琐碎用法]()
[6.1 jupyter的cell可以作为unix command使用]()
具体方法为：在unitx command前面加入一个感叹号“！”
例子： 
查看python版本：!python --version 
运行python文件：!python myfile.py
[6.2 Magic functions]()
还没有太明白，具体细节见[The cell magics in IPython](https://link.jianshu.com?t=http%3A%2F%2Fnbviewer.jupyter.org%2Fgithub%2Fipython%2Fipython%2Fblob%2F1.x%2Fexamples%2Fnotebooks%2FCell%2520Magics.ipynb%23The-cell-magics-in-IPython)
[6.3 获取current working directory]()
即当前运行的代码所在的路径 
具体方法：current_path = %pwd 
这样得到的current_path就是当前工作路径的字符转
[6.4 使用Matplotlib绘图]()
在Jupyter Notebook中，如果使用Matplotlib绘图，有时是弹不出图像框的，此时，可以在开头加入
%matplotlibinline
1
[七、Jupyter中的Markdown]()
[参考1](https://link.jianshu.com?t=http%3A%2F%2Fsebastianraschka.com%2FArticles%2F2014_ipython_internal_links.html)
[Markdown 语法手册 （完整整理版）参考](https://link.jianshu.com?t=http%3A%2F%2Fblog.leanote.com%2Fpost%2Ffreewalk%2FMarkdown-%25E8%25AF%25AD%25E6%25B3%2595%25E6%2589%258B%25E5%2586%258C)
[7.1 在jupyter中设置link，需要设置两部分：]()
要跳到的位置(the destination) 
需要在要跳转到的位置添加下面语句:
1
这里的id取值任意赋值，下面在添加链接时要用
需要添加链接的文字（an internal hyperlink to the destination），即点击该处可以跳转到the destination，在需要添加链接的文字后面加入：
[需要添加连接的文字](#the_destination)
1
下面是一个例子： 
源码： 
![](https://upload-images.jianshu.io/upload_images/8344711-38b59afa7e85479f?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
效果图： 
![](https://upload-images.jianshu.io/upload_images/8344711-59b1a980dd336999?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
[7.2 为Jupyter Notebook添加目录功能]()
原始的Jupyter是不支持markdown添加目录功能的
实际上，可以利用[Jupyter notebook extensions](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fipython-contrib%2Fjupyter_contrib_nbextensions)去使得这种功能实现
具体方法： 
利用Anaconda安装Jupyter Notebook extensions 
conda install -c conda-forge jupyter_contrib_nbextensions 
![](https://upload-images.jianshu.io/upload_images/8344711-9ec1c0620cf9fd80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
打开Jupyter Notebook，在它的（新增的）Nbextensions标签下勾选“Table of Contents(2)” 
![](https://upload-images.jianshu.io/upload_images/8344711-4602a4bd085a2ad9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
打开一个.jpynb文件，发现，目录功能可用了！ 
![](https://upload-images.jianshu.io/upload_images/8344711-7e095b88180fa40d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
作者：python_Studio
链接：https://www.jianshu.com/p/21ba32a057c4
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
