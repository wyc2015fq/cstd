# Ubuntu下apt-get install安装与pip install安装的区别 - 数据之美的博客 - CSDN博客
2017年12月18日 16:37:04[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：13613
#### 在下载Python依赖包的时候，突然发现有些人的执行命令是sudo pip install pysftp,也有人用sudo apt-get install pysftp,所以很好奇它们这两者到底区别在哪儿？什么时候该用pip,什么时候该用apt-get呢？该如何安装和使用apt-get与pip?
现在，我就把自己学到的干货和大家分享下：
1. pip与apt-get的区别在哪儿呢？
pip用来安装来自PyPI的python所有的依赖包，并且可以选择安装任何在PyPI上已上传的先前版本的依赖包； 
apt-get可以用来安装软件、更新源、也可以用来更新自Ubuntu的典型依赖包，典型安装即意味着它只是安装（最新发布的，或最近一个的）单一版本，并且我们不能决定我们要安装的依赖包的版本或选择它之前的版本。
2. 什么时候该用pip,什么时候该用apt-get呢？
情况是这样的，如果你需要最新版本的python依赖包，你可以直接使用apt-get,在项目突然需要使用旧版本的依赖包时，你就可以使用virtualenv和pip来使完美得再安装上一个旧版本的依赖包； 
你可以根据你的喜好选择你喜欢的安装方式。但是，如果你需要安装python包的特定版本，或在virtualenv中安装包，或安装只托管PyPI上一个包，此时只有pip会帮你解决这个问题了。否则，如果你不介意安装在系统范围的位置使用apt-get或pip都是ok的；
3. 该如何安装和使用apt-get与pip?
apt-get是可以直接使用的，格式为
`sudo apt-get install/delete package`- 1
```bash
sudo apt-get -f install                                   #修复安装
```
- 1
```bash
sudo apt-get dist-upgrade                                 #升级系统
```
- 1
```bash
sudo apt-get upgrade                                      #更新已安装的包
```
- 1
```bash
apt-get source package                                    #下载该包的源代码
```
- 1
```bash
sudo apt-get build-dep package                            #安装相关的编译环境
```
- 1
```bash
sudo apt-get clean && sudo apt-get autoclean              #清理无用的包
```
- 1
pip需要安装才能使用，配合virtualenv会锦上添花。安装过程如下（适用Ubuntu 10.10及以上版本），使用格式为：pip install package。
`$ sudo apt-get install python-pip python-dev build-essential `- 1
```bash
$ sudo pip install --upgrade pip
```
- 1
```bash
$ sudo pip install --upgrade virtualenv
```
- 1
注：virtualenv是一个python工具，它可以创建一个独立的python环境，这样做的好处是你的python程序运行在这个环境里，不受其它python library的版本问题影响。
