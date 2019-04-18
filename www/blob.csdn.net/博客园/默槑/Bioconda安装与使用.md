# Bioconda安装与使用 - 默槑 - 博客园







# [Bioconda安装与使用](https://www.cnblogs.com/modaidai/p/7414051.html)





1、  Bioconda是一个自动化管理生物信息软件的工具，就像APPstore、360软件管家一样。

　　Bioconda的优点是安装简单，各个软件依赖的环境一同打包且相互隔离，非常适合在服务器中建立自己的生物信息分析环境

　　1.下载和安装miniconda 

　　　　bioconda的使用首先需要安装miniconda　　
`　　　wget https://repo.continuum.io/miniconda/Miniconda3-4.3.21-Linux-x86_64.sh`
　　　　下载完成后，在终端键入bash命令进行安装：

`bash Miniconda-latest-Linux-x86_64.sh`


```
之后按照提示点击回车，输入要安装的位置，或者输入yes
```

　　　　输入yes后，还没有完成最后安装，还需要source一下：


```
source ~/.bashrc
　　  
　　　输入“conda list”来查看已经安装的软件：
```

　　2.添加channels


```
conda config --add channels conda-forge
　　　　conda config --add channels defaults
  　　　conda config --add channels r
　　　　conda config --add channels bioconda
```


　　　　　 查看已经添加的channels：

`　　　　conda config --get channels`

　　3.更新miniconda

`　　  conda update conda　　`
　　4.卸载miniconda
　　　　删除miniconda的整个文件夹：


```
rm -rf ~/miniconda        #这个命令不靠谱，需要去root文件夹中确认要删除的miniconda文件夹的名称
　　  从环境变量中去掉miniconda：打开~/.bashrc文件，删掉最下面的miniconda的路径，关闭并保存
```
`   5.`利用Bioconda安装生物信息软件
　　　　 要通过conda安装软件，首先从这里（[Available packages](http://link.zhihu.com/?target=https%3A//bioconda.github.io/recipes)）查找该软件是否被conda支持。如果支持，只需输入以下命令即可安装：


```
conda install fastqc（软件名）
```

　　　　安装完成后，可以用“which 软件名”来查看该软件安装的位置：

　　　　或者再次查看已安装软件列表“conda list”



　　　　conda默认安装软件的最新版本，如果想安装指定版本的某个软件，可以先用“conda search 软件名”搜索软件版本



　　　　星号标记的表示是已经安装的版本。要安装其他版本，输入：


```
conda install 软件名=版本号
```


　　　　这时conda会先卸载已安装版本，然后重新安装指定版本。



　　　　更新指定软件：


```
conda update 软件名
```


　　　　卸载指定软件：

`　　　　conda remove 软件名`


` `



` `












