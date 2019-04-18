# 在pycharm中配置Anaconda以及pip源配置 及更新 - fighting！！！ - CSDN博客
2019年01月08日 18:37:01[dujiahei](https://me.csdn.net/dujiahei)阅读数：106
转自：[https://www.cnblogs.com/jpfss/p/9192641.html](https://www.cnblogs.com/jpfss/p/9192641.html)
在学习推荐系统、机器学习、数据挖掘时，python是非常强大的工具，也有很多很强大的模块，但是模块的安装却是一件令人头疼的事情。
现在有个工具——anaconda，他已经帮我们集成好了很多工具了！anaconda里面集成了很多关于python科学计算的第三方库，主要是安装方便，而python是一个编译器，如果不使用anaconda，那么安装起来会比较痛苦，各个库之间的依赖性就很难连接的很好。
在windows中，pycharm是一个比较好python编辑器，所以如果能把pycharm 和 anaconda结合起来，岂不是美哉！
**1.下载安装anaconda**
[点击这里下载](https://www.continuum.io/downloads#windows)anaconda，注意对应自己想要安装的python版本就行了。
这里还要注意一个**问题**：因为anaconda是自带Python的，所以不需要自己再去下载安装Python了，当然，如果你已经安装了Python也不要紧，不会发生冲突的！
**2.下载安装pycharm**
建议使用这个编辑器吧，个人感觉还是很好用的。如果你是已经安装了pycharm了，那么可以直接跳到下一步。[点击这里下载pycharm](http://www.jetbrains.com/pycharm/download/#section=windows)
它会自动找到你安装的python，安装时按提示来就是了。
**3.在pycharm中配置anaconda的解释器**
具体做法是：File->Default settings->Default project->project interpreter
接着点击 project interpreter 的右边的小齿轮，选择 add local ，选择anaconda文件路径下的python.exe。接着pycharm会更新解释器，导入模块等，要稍等一点时间。
好了，到目前为止，anaconda在pycharm中的配置就基本完成了。难道我们就要满足使用conda中的那些包了吗？并不是，conda为我们带来了更多的东西，使我们在管理Python库的时候更加方便快捷！接下来就让我来详细为你解说一下吧！
### 1、配置国内**源**
让我们先来解决第一个问题，相信这个问题也是让很多人都感到头疼的，因为如果不进行配置的话，安装python库的时候默认是使用国外的源，这时候下载速度会很慢！国内的源下载速度要好很多。
**pip源配置**
大家比较熟悉的可能是使用pip来安装python的库（也有可能是easy install，不过我这里只讲pip的配置），所以就先来解决pip的源吧。
**注意配置环境**windows7 （64位），Python3.6
- 在windows文件管理器中，输入`%APPDATA%`,回车
- 接着会定位到一个新的目录，在这个目录中新建一个pip文件夹，然后在pip文件夹中新建个pip.ini文件
- 最后再新建的pip.ini文件中输入一下内容：
```
[global]
index-url = https://pypi.tuna.tsinghua.edu.cn/simple
```
好了，到目前为止，pip源的配置就搞定了。
**anaconda的源配置**
在安装了anaconda后，我们也可以使用anaconda来进行Python库的安装，同样的也需要进行源的配置。（其实使用pip，anaconda来进行Python库的安装都是差不多，不过个人比较喜欢用anaconda）
这个配置方法就很简单了，你只需要在配置了anaconda的pycharm中的终端（Terminal）逐条输入以下两条命令即可：
```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```
好了，源的配置到此就完成了。大家有兴趣可以去清华大学的[开源镜像站](https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/)看看，会有很多惊喜的。
### 2、conda的包管理
anaconda为我们提供方便的包管理命令——conda, 下面我们来看看都有哪些有用的命令吧！
```
# 查看已经安装的packages
conda list
# 查看某个指定环境的已安装包
conda list -n python34
# 查找package信息
conda search numpy
# 安装scipy
conda install scipy
# 安装package
# 如果不用-n指定环境名称，则被安装在当前活跃环境
conda install -n python34 numpy
# 更新package
conda update -n python34 numpy
# 删除package
conda remove -n python34 numpy
```
由于conda将conda、python等都视为package，因此，完全可以使用conda来管理conda和python的版本，例如
```
# 更新conda，保持conda最新
conda update conda
# 更新anaconda
conda update anaconda
# 更新python
conda update python
```
