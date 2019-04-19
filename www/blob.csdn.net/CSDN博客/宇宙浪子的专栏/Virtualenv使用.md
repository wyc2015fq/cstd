# Virtualenv使用 - 宇宙浪子的专栏 - CSDN博客
2016年02月17日 16:51:54[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：4304
转自：[http://blog.csdn.net/lihao21/article/details/50385766](http://blog.csdn.net/lihao21/article/details/50385766)
# 一、简介
Virtualenv是一个用来创建隔离的python虚拟环境的工具。 
我们知道，不同的Python程序可能依赖同一个python包的不同版本。这时，我们不能简单地全局安装或者全局升级这个包，因为这样做会导致所有依赖这个包的Python程序都受到影响。另外，由于权限等原因，我们可能不能直接在全局的`site-packages`目录安装python包，这时virtualenv也能派上用场。
Virtualenv可以在一个独立的目录创建自己的python环境，使用virtualenv运行的程序，并不会访问全局的python环境，也不会访问不属于自己目录的python环境，这样一来，可以起到隔离python环境的作用。
# 二、安装
如果需要安装virtualenv，可以使用pip直接安装：
```python
pip install virtualenv
```
推荐先升级pip后再安装virtualenv。
# 三、使用
Virtualenv有个最基本的用法：
```python
virtualenv ENV
```
执行此命令后，virtualenv在目录ENV下创建一个新的python虚拟环境。目录ENV包含以下的内容：
- bin目录，一个新的python运行环境
- lib和include目录，包括了虚拟环境所需的库。可以看到安装的python包被放到目录lib/pythonX.X/site-packages/下
- pip和setuptools包已经安装好，可以用来方便地为该虚拟环境安装新的python包。可以使用ENV/bin/pip来直接指定该pip安装python包
目录ENV下的虚拟环境是独立的，即跟创建ENV虚拟环境的python环境也是独立的。
## 1. 激活虚拟环境
为了激活创建的虚拟环境，可以在目录ENV下使用以下命令：
```python
source bin/activate
```
执行此命令后，可以看到命令提示符前面增加了(ENV)的提示符，表明当前使用ENV的python环境。还可以看到，`$PATH`变量已经将虚拟环境目录下的bin/目录添加进去。 
这个命令也仅是为了使用的方便而已，如果你直接使用ENV/bin/pip来安装python包，也可以不使用activate命令来激活虚拟环境。
## 2. 为虚拟环境安装python包
进入虚拟环境后，可以使用pip来为该虚拟环境安装python包，新安装的包会被放置在目录ENV/lib/pythonX.X/下。
## 3. 在虚拟环境下运行python程序
进入虚拟环境后，可以直接使用`python 程序名.py`的方式来运行程序。这时的程序会使用虚拟环境的python包。 
也可以直接使用指定虚拟环境目录下的python来运行程序，这时的程序也是使用虚拟的python环境。例如
```python
ENV/bin/python hello.py
```
使用这种方式，可以方便地使用virtualenv + supervisor来布署线上生产环境的python程序。
## 4. 退出虚拟环境
可以使用以下命令退出虚拟环境：
```python
deactivate
```
退出后，会继续使用全局的python环境。
# 四、参考资料
- [https://virtualenv.readthedocs.org/en/latest/](https://virtualenv.readthedocs.org/en/latest/)
- [https://virtualenv.readthedocs.org/en/latest/installation.html](https://virtualenv.readthedocs.org/en/latest/installation.html)
- [https://virtualenv.readthedocs.org/en/latest/userguide.html](https://virtualenv.readthedocs.org/en/latest/userguide.html)
- [](https://virtualenv.readthedocs.org/en/latest/userguide.html)
