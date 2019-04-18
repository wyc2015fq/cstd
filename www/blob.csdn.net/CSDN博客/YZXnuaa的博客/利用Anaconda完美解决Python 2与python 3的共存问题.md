# 利用Anaconda完美解决Python 2与python 3的共存问题 - YZXnuaa的博客 - CSDN博客
2018年04月26日 17:56:30[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：326
**前言**
现在Python3 被越来越多的开发者所接受，同时让人尴尬的是很多遗留的老系统依旧运行在 Python2 的环境中，因此有时你不得不同时在两个版本中进行开发，调试。
如何在系统中同时共存 Python2 和 Python3 是开发者不得不面对的问题，一个利好的消息是，Anaconda 能完美解决Python2 和 Python3 的共存问题，而且在 Windows 平台经常出现安装依赖包（比如 MySQL-python）失败的情况也得以解决。
**Anaconda 是什么？**
Anaconda 是 Python 的一个发行版，如果把 Python 比作 Linux，那么 Anancoda 就是 CentOS 或者 Ubuntu。它解决了Python开发者的两大痛点。
- 第一：提供包管理，功能类似于 pip，Windows 平台安装第三方包经常失败的场景得以解决。
- 第二：提供虚拟环境管理，功能类似于 virtualenv，解决了多版本Python并存问题。
**下载 Anaconda安装包**
**官网下载地址：**[https://www.continuum.io/downloads](https://www.continuum.io/downloads)
**本地下载地址：**
Anaconda3 4.3.1 Python 3.6 for windows 64位: [http://www.jb51.net/softs/556361.html](http://www.jb51.net/softs/556361.html)
Anaconda3 4.3.1 Python 3.6 for windows 32位: [http://www.jb51.net/softs/556363.html](http://www.jb51.net/softs/556363.html)
Anaconda3 4.3.1 Python 3.6 for linux   32位: [http://www.jb51.net/softs/556380.html](http://www.jb51.net/softs/556380.html)
Anaconda3 4.3.1 Python 3.6 for linux   64位: [http://www.jb51.net/softs/556392.html](http://www.jb51.net/softs/556392.html)
我们选择 Python3.6 版本的安装包，下载完成后直接安装，安装过程选择默认配置即可，大约需要1.8G的磁盘空间。
conda 是 Anaconda 下用于包管理和环境管理的命令行工具，是 pip 和 vitualenv 的组合。安装成功后 conda 会默认加入到环境变量中，因此可直接在命令行窗口运行 conda 命令
如果你熟悉 virtualenv，那么上手 conda 非常容易，不熟悉 virtulenv 的也没关系，它提供的命令就几个，非常简单。我们可以利用 conda 的虚拟环境管理功能在 Python2 和 Python3 之间自由切换。
**多版本切换**
[?](http://www.jb51.net/article/114614.htm#)
```
`# 基于 python3.6 创建一个名为test_py3 的环境`
`conda create --name test_py3 python=3.6 `
`# 基于 python2.7 创建一个名为test_py2 的环境`
`conda create --name test_py2 python=2.7`
`# 激活 test 环境`
`activate test_py2 # windows`
`source activate test_py2 # linux/mac`
`# 切换到python3`
`activate test_py3`
```
更多命令，可查看帮助 `conda -h`
**包管理工具**
conda 的包管理功能是对 pip 的一种补充，如果当前已经激活了某个Python环境，那么就可以在当前环境开始安装第三方包。
[?](http://www.jb51.net/article/114614.htm#)
```
`# 安装 matplotlib `
`conda install matplotlib`
`# 查看已安装的包`
`conda list `
`# 包更新`
`conda update matplotlib`
`# 删除包`
`conda remove matplotlib`
```
对于那些用 pip 无法安装成功的模块你都可以尝试用 conda 来安装，如果用 conda 找不到相应的包，当然你继续选择 pip 来安装包也是没问题的。
**提高下载速度**
Anaconda 的镜像地址默认在国外，用 conda 安装包的时候会很慢，目前可用的国内镜像源地址是清华大学提供的。修改 ~/.condarc (Linux/Mac) 或 C:\Users\当前用户名.condarc (Windows) 配置
[?](http://www.jb51.net/article/114614.htm#)
```
`channels:`
` ``- [https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/)`
` ``- defaults`
`show_channel_urls: true`
```
除此之外，你也可以把 pip 的镜像源地址也换成国内的，豆瓣源速度比较快。修改 ~/.pip/pip.conf (Linux/Mac) 或 C:\Users\当前用户名\pip\pip.ini (Windows) 配置：
[?](http://www.jb51.net/article/114614.htm#)
```
`[global]`
`trusted-host = pypi.douban.com`
`index-url = [http://pypi.douban.com/simple](http://pypi.douban.com/simple)`
```
**总结**
以上就是这篇文章的全部内容，希望本文的内容对大家的学习或者工作能带来一定的帮助，如果有疑问大家可以留言交流，谢谢大家对脚本之家的支持。            
