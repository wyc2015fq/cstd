# python︱模块加载(pip安装)以及pycharm安装与报错解决方式 - 素质云笔记/Recorder... - CSDN博客





2016年08月14日 17:30:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：44193












**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


      准备放下R开始学python,真是痛苦，因为找个IDE都好麻烦，调用起来都没Rsudio那么好用。这个IDE下载模块比较方面，非常快。比较好的有pycharm以及Anaconda这个组合模组。一般来说，在window环境下加载模组，可谓是恐怖之极的事情。

      安装完anaconda，就相当于安装了Python、IPython、集成开发环境Spyder、一些包等等。


具体可参考：

# [python︱Anaconda安装、简介（安装报错问题解决、Jupyter Notebook）](http://blog.csdn.net/sinat_26917383/article/details/52933407)



——————————————————————————————






# 一、模组、模块导入与复查






一般笔者看到有以下的几类模组安装方式：

1、直接找到模组.exe类文件，然后加载；

2、pip的方式，不过也需要下载相应的材料，一般从[sourceforce](https://sourceforge.net/)下载；

3、其他IDE自带加载功能，pycharm和Anaconda就是这样的可以帮你加载包的内容，其中Anaconda更是有很多用处。







## 1、module的查看

下载模块是一个麻烦的事情，一般用pip来执行，但是貌似每次下载都是一堆麻烦提示，于是转而用pycharm，很方面，傻瓜版.

一般模块就像R中的函数包，需要先调用





**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52289476#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[?](http://blog.csdn.net/sinat_26917383/article/details/52289476#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- library(packages)=import pandas as pd  


查看模块是否载入，一般import pandas，如果该包下载就不会用任何提示，如果没有加载成功，就会报错：






**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52289476#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[?](http://blog.csdn.net/sinat_26917383/article/details/52289476#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- ImportError: No module named da  


查看已有的加载包





**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52289476#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52289476#)[?](http://blog.csdn.net/sinat_26917383/article/details/52289476#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- help("modules")    #查看安装包  





按照R语言中曾经存在的问题：

1、如何取消模块的加载?

2、模块的位置是在哪?

3、模块的信息如何调用出来？就像R中的介绍一样，有没有比较详细的说明？


## 2、pip的安装

pip的安装，貌似在window环境下也是一堆Bug。我将以提问的方式来安装pip。主要参考博客：[python实践系列之（一）安装 python/pip/numpy/matplotlib](http://blog.csdn.net/sinat_28224453/article/details/51462935)




### （1）如何打开cmd命名窗口？




尼玛，对于电脑小白，操作CMD真是小怕怕...win+R之后会跳出运行框框，然后输入”cmd“

![](https://img-blog.csdn.net/20161026104734627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### （2）如何检查自己有没有安装pip？



#### `输入：pip --version`
如果安装，则会显示版本，如果没有安装会报错：


"pip"不是内部或外部命令，也不是可运行程序，或批量文件。




### （3）安装pip



#### 安装pip前需确认已安装Python和easy_install。

#### 如果成功装了python，则easy_install在Python安装目录下的scripts文件夹下，如，D:\softwares_diy\Python34\Scripts。


#### 进入Python安装目录下的Scripts路径，执行 `easy_install.exe pip`




咦？ 为什么一直保持在C\users\long\>?如何修改目录至python安装目录呢？

这里需要在：C\users\long\>

后面输入：”F：“就可以转到F盘的目录了。具体内容参靠下图输入的方式。

![](https://img-blog.csdn.net/20161026105930773)





### （4）pip版本的升级

在CMD中执行：pip install pip --upgrade




## 3、用pip安装譬如numpy的模块方式



#### 将下载的numpy包拷贝到D:\softwares_diy\Python34\Scripts下（不知道怎么回事只有在这个目录下 pip安装命令才好使）

再执行




`pip install “numpy-1.10.4+mkl-cp34-cp34m-win_amd64.whl”`若出现图3-2的情况，


![这里写图片描述](https://img-blog.csdn.net/20160520162827616)
图3-2

则按提示修改，进入Python安装目录，如，D:\softwares_diy\Python34，

输入`python -m pip install --upgrade pip`

![这里写图片描述](https://img-blog.csdn.net/20160520162840315)
图3-3

很快出现如图3-4，成功升级至pip 8.1：

![这里写图片描述](https://img-blog.csdn.net/20160520162902163)
图3-4

## 3、以numpy包安装为例

先网上下载安装包，然后pip执行一下。

在sourceforge上可以下载到，64位Windows 下Python3.4.4的下载地址，[numpy-1.10.4+mkl-cp34-cp34m-win_amd64.whl](http://www.lfd.uci.edu/~gohlke/pythonlibs/6kbpejrn/numpy-1.10.4+mkl-cp34-cp34m-win_amd64.whl) 或 [numpy-1.11.0+mkl-cp34-cp34m-win_amd64.whl](http://www.lfd.uci.edu/~gohlke/pythonlibs/6kbpejrn/numpy-1.11.0+mkl-cp34-cp34m-win_amd64.whl)

如果是其他版本的Python，则下载对应的numpy，如下图，在 [Python extension packages](http://www.lfd.uci.edu/~gohlke/pythonlibs/) 页面下载对应Windows位数（32/64）、Python版本（2.7/3.4/3/5）的numpy包。

#### 将下载的numpy包拷贝到D:\softwares_diy\Python34\Scripts下（不知道怎么回事只有在这个目录下 pip安装命令才好使）
再执行：pip install “numpy-1.10.4+mkl-cp34-cp34m-win_amd64.whl”




## 4、matplotlib的安装

该包的安装可谓真是麻烦...需要很多的基础包，还需要安装dateutil，pyparsing，pytz，six

一样的是，都要找到这些内容的安装包，然后pip install python-dateutil

当然，也可以直接用pycharm、Anaconda来快速获取。不过，pycharm在安装的时候，还是遇到一堆麻烦。




——————————————————————————————





# 二、Python、Pycharm的安装






pycharm的下载与pandas安装：http://bbs.pinggu.org/thread-3633477-1-1.html

pycharm官方教学视频：链接：http://pan.baidu.com/s/1sl3WfGL 密码：03ho

Pycharm中的一些快捷键使用：http://www.jb51.net/article/74371.htm


从Python官网下载windows版本的Python，要下载python2，不要下载python3。鉴于大家可能上了官网都不知道下载哪个版本（我自己一开始就是），我已经将安装文件上传到百度云，下载地址：
Python 2.7.9 32位下载地址
链接: [http://pan.baidu.com/s/1c0o4R0c](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cKGt7c4N5rokneYbnMVuobkNnbztxek1y6J3/k6ikqKTufQOAF1iNSkjiCsXpLQC/sqC/Es94ADFQFGM3yjN1R41iRi160rIkh2Pah4egTNMg==) 密码:zzdc
Python 2.7.9 64位下载地址
链接: [http://pan.baidu.com/s/1ntyvVC1](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cJb4zklBzQwImVjZ4PKEs+7Nnbztxek1y6J3/k6ikqKTufQOAF1iNSkjiCsXpLQC/sqC/Es94ADFQFGM3yjN1R41iRi160rIkh2Pah4egTNMg==) 密码:1bt7
安装文件下载好之后，就像一般的软件一样，一直点击next就能安装。


Pycharm是一个ide，说简单点就是一个用来编写Python程序的软件，也是个神器，可以让我们更加方便的编写Python程序。我们后面安装pandas就需要用到它。Pycharm有收费版本也有免费版本，我们用免费版本就已经绰绰有余了。可以去官网下载，因为是国外的网站，下载速度可能很慢，我已经将安装文件上传到百度云，下载地址:[http://pan.baidu.com/s/1dDtm8Id](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cIuHuwoIn5FlY7WhddXy4hONnbztxek1y6J3/k6ikqKTufQOAF1iNSkjiCsXpLQC/sqC/Es94ADFQFGM3yjN1R41iRi160rIkh2Pah4egTNMg==) 密码:r6f9
下载好之后一路点击next就能安装好。





调出方式：

settings——project interpreter——然后点加号就可以添加模块了。




在搜索框中输入pandas，点击最下方的Install Package，根据网速快慢，等待不定的时间，就完成了安装，如下图所示
![](https://img-blog.csdn.net/20161025221415051)

安装过程中可能会提示：Microsoft Visual C++ 9.0 is required (Unable to find vcvarsall.bat). Get it from[http://aka.ms/vcpython27](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvlQnangRvIiQU4axZnBrEr19GJkNyDrS1+5delI/cFQdgjR212fC9/HrSsUS9wqwC1ZQ47UjWzXaprEx985WqrmmxsNAS+b72GR8/dzRORwcqrwRgpxE70MDxm7iZ2BjQ=)
只要按照提示说的的去下载一个VC就可以了。这样就安装好了pandas。
然后在按照同样的流程安装好其他一些pandas依赖的package，分别是：numpy，dateutil，可能还有其他。
安装好之后试着运行下面这个程序，如果没有报错，就说明完全安装好pandas了。如果报错，一般是因为缺少依赖的库，按照上述方法继续安装就可以了。










——————————————————————————————————————————————




# 问题一：pycharm在添加模块时候遇到问题




添加了还是报错no module，这时候你看一下上图中有一个选项是：

**install to user's site packages directory.**...

这个选项如果勾选了的话，代表着安装在了其他地方，你可以找到这个目录，然后把里面的内容，

复制到python根目录下的lib里面，这样就可以调用了。

![](https://img-blog.csdn.net/20161025215015660)





——————————————————————————————————————————————





# **问题二：安装module之后，加载还是失败**

也会出现这样的情况，或者可以重启一下。




——————————————————————————————





# 问题三：debug pip.log


`Storing debug log for failure in C:\Users\long\pip\pip.log`

这个问题，我现在还是一样的无解...一般可能是预安装的包没有安装上？还是其他原因？




——————————————————————————————





# 问题四：module object no attribute


`AttributeError: 'module' object has no attribute 'cbook'`
这个报错好像经常会出现，网上叙述这个貌似主要有两个问题：module的路径问题，module文件问题。




最近在编写Python脚本过程中遇到一个问题比较奇怪：Python脚本完全正常没问题，但执行总报错"AttributeError: 'module' object has no attribute 'xxx'"。这其实是.pyc文件存在问题。

问题定位：

查看import库的源文件，发现源文件存在且没有错误，同时存在源文件的.pyc文件



问题解决方法：

1. 命名py脚本时，不要与python预留字，模块名等相同

2. 删除该库的.pyc文件（因为py脚本每次运行时均会生成.pyc文件；在已经生成.pyc文件的情况下，若代码不更新，运行时依旧会走pyc，所以要删除.pyc文件），重新运行代码；或者找一个可以运行代码的环境，拷贝替换当前机器的.pyc文件即可
（参考博文：[Python脚本报错AttributeError: ‘module’ object has no attribute’xxx’解决方法](http://www.cnblogs.com/tl542475736/p/5614058.html)）
3、把你的源文件re.py换个文件名, 不要用跟系统库同样名字.

以上的解决办法，都可以试试。







**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




